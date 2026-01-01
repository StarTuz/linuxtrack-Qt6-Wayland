/*
 * ltr_wine_hotkeys.c - Native Win32 Hotkey Utility for Linuxtrack
 * 
 * Captures global hotkeys using low-level keyboard hook and sends 
 * UDP commands ("RECN", "PAUS") to the Linuxtrack bridge DLL on localhost:4242.
 * 
 * Uses WH_KEYBOARD_LL hook instead of RegisterHotKey for better 
 * compatibility with Wine/Proton under Wayland/XWayland.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#define IDC_BTN_RECENTER 101
#define IDC_BTN_PAUSE    102
#define IDC_ST_RECENTER  103
#define IDC_ST_PAUSE     104

/* Config */
UINT cfg_vk_recenter = VK_F12;
UINT cfg_vk_pause    = VK_PAUSE;

char ini_path[MAX_PATH] = "";

SOCKET client_sock = INVALID_SOCKET;
struct sockaddr_in bridge_addr;

enum { STATE_IDLE, STATE_BIND_RECENTER, STATE_BIND_PAUSE };
int bind_state = STATE_IDLE;

HWND hMain;
HWND hRecenterBtn, hPauseBtn;
HWND hRecenterText, hPauseText;
HHOOK hKeyboardHook = NULL;

void init_ini_path(HINSTANCE hInst) {
    GetModuleFileNameA(hInst, ini_path, MAX_PATH);
    char *last_slash = strrchr(ini_path, '\\');
    if (last_slash) {
        strcpy(last_slash + 1, "ltr_hotkeys.ini");
    } else {
        strcpy(ini_path, "ltr_hotkeys.ini");
    }
}

void send_command(const char *cmd) {
    if (client_sock == INVALID_SOCKET) return;
    sendto(client_sock, cmd, 4, 0, (struct sockaddr*)&bridge_addr, sizeof(bridge_addr));
}

void load_config() {
    cfg_vk_recenter = GetPrivateProfileIntA("Hotkeys", "Recenter", VK_F12, ini_path);
    cfg_vk_pause    = GetPrivateProfileIntA("Hotkeys", "Pause", VK_PAUSE, ini_path);
}

void save_config() {
    char buf[16];
    sprintf(buf, "%u", cfg_vk_recenter);
    WritePrivateProfileStringA("Hotkeys", "Recenter", buf, ini_path);
    sprintf(buf, "%u", cfg_vk_pause);
    WritePrivateProfileStringA("Hotkeys", "Pause", buf, ini_path);
}

void update_labels() {
    char buf[64];
    char key_name[64];
    
    GetKeyNameTextA(MapVirtualKeyA(cfg_vk_recenter, MAPVK_VK_TO_VSC) << 16, key_name, 64);
    sprintf(buf, "Recenter Key: %s", key_name);
    SetWindowTextA(hRecenterText, buf);
    
    GetKeyNameTextA(MapVirtualKeyA(cfg_vk_pause, MAPVK_VK_TO_VSC) << 16, key_name, 64);
    sprintf(buf, "Pause Key: %s", key_name);
    SetWindowTextA(hPauseText, buf);
    
    if (bind_state == STATE_BIND_RECENTER) SetWindowTextA(hRecenterText, "Press any key...");
    if (bind_state == STATE_BIND_PAUSE)    SetWindowTextA(hPauseText, "Press any key...");
}

/* Low-level keyboard hook callback */
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *kbd = (KBDLLHOOKSTRUCT *)lParam;
        UINT vk = kbd->vkCode;
        
        /* Handle rebinding mode */
        if (bind_state != STATE_IDLE) {
            if (bind_state == STATE_BIND_RECENTER) cfg_vk_recenter = vk;
            if (bind_state == STATE_BIND_PAUSE)    cfg_vk_pause = vk;
            bind_state = STATE_IDLE;
            save_config();
            PostMessage(hMain, WM_USER + 1, 0, 0); /* Signal to update labels */
            return 1; /* Consume the key */
        }
        
        /* Check for configured hotkeys */
        if (vk == cfg_vk_recenter) {
            send_command("RECN");
        } else if (vk == cfg_vk_pause) {
            send_command("PAUS");
        }
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            hRecenterText = CreateWindowA("STATIC", "Recenter Key: F12", WS_VISIBLE | WS_CHILD, 10, 10, 200, 20, hwnd, (HMENU)IDC_ST_RECENTER, NULL, NULL);
            hRecenterBtn  = CreateWindowA("BUTTON", "Rebind", WS_VISIBLE | WS_CHILD, 220, 5, 60, 25, hwnd, (HMENU)IDC_BTN_RECENTER, NULL, NULL);
            
            hPauseText    = CreateWindowA("STATIC", "Pause Key: Pause", WS_VISIBLE | WS_CHILD, 10, 40, 200, 20, hwnd, (HMENU)IDC_ST_PAUSE, NULL, NULL);
            hPauseBtn     = CreateWindowA("BUTTON", "Rebind", WS_VISIBLE | WS_CHILD, 220, 35, 60, 25, hwnd, (HMENU)IDC_BTN_PAUSE, NULL, NULL);
            update_labels();
            break;
            
        case WM_COMMAND:
            if (LOWORD(wp) == IDC_BTN_RECENTER) {
                bind_state = STATE_BIND_RECENTER;
                update_labels();
            } else if (LOWORD(wp) == IDC_BTN_PAUSE) {
                bind_state = STATE_BIND_PAUSE;
                update_labels();
            }
            break;

        case WM_USER + 1: /* Update labels signal from hook */
            update_labels();
            break;

        case WM_DESTROY:
            if (hKeyboardHook) {
                UnhookWindowsHookEx(hKeyboardHook);
                hKeyboardHook = NULL;
            }
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    /* Single-instance check using a named mutex */
    HANDLE hMutex = CreateMutexA(NULL, TRUE, "LtrWineHotkeysMutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        if (hMutex) CloseHandle(hMutex);
        return 0;
    }
    
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    
    client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset(&bridge_addr, 0, sizeof(bridge_addr));
    bridge_addr.sin_family = AF_INET;
    bridge_addr.sin_port = htons(4243);
    inet_pton(AF_INET, "127.0.0.1", &bridge_addr.sin_addr);

    init_ini_path(hInst);
    load_config();

    /* Install low-level keyboard hook */
    hKeyboardHook = SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0);
    if (!hKeyboardHook) {
        MessageBoxA(NULL, "Failed to install keyboard hook", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = "LtrHotkeyUtility";
    RegisterClassA(&wc);

    hMain = CreateWindowExA(WS_EX_TOOLWINDOW, "LtrHotkeyUtility", "Linuxtrack Hotkeys", 
                             WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 
                             CW_USEDEFAULT, CW_USEDEFAULT, 300, 110, NULL, NULL, hInst, NULL);
    
    ShowWindow(hMain, SW_SHOW);
    UpdateWindow(hMain);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    if (hKeyboardHook) UnhookWindowsHookEx(hKeyboardHook);
    closesocket(client_sock);
    WSACleanup();
    return 0;
}
