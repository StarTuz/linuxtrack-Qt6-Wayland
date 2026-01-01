/*
 * ltr_wine_hotkeys.c - Native Win32 Hotkey Utility for Linuxtrack
 * 
 * Captures global hotkeys and sends UDP commands ("RECN", "PAUS") 
 * to the Linuxtrack bridge DLL on localhost:4242.
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

#define HOTKEY_ID_RECENTER 1
#define HOTKEY_ID_PAUSE    2

/* Config */
UINT cfg_vk_recenter = VK_F12;
UINT cfg_vk_pause    = VK_PAUSE;

SOCKET client_sock = INVALID_SOCKET;
struct sockaddr_in bridge_addr;

enum { STATE_IDLE, STATE_BIND_RECENTER, STATE_BIND_PAUSE };
int bind_state = STATE_IDLE;

HWND hMain;
HWND hRecenterBtn, hPauseBtn;
HWND hRecenterText, hPauseText;

void send_command(const char *cmd) {
    if (client_sock == INVALID_SOCKET) return;
    sendto(client_sock, cmd, 4, 0, (struct sockaddr*)&bridge_addr, sizeof(bridge_addr));
}

void load_config() {
    cfg_vk_recenter = GetPrivateProfileIntA("Hotkeys", "Recenter", VK_F12, ".\\ltr_hotkeys.ini");
    cfg_vk_pause    = GetPrivateProfileIntA("Hotkeys", "Pause", VK_PAUSE, ".\\ltr_hotkeys.ini");
}

void save_config() {
    char buf[16];
    sprintf(buf, "%u", cfg_vk_recenter);
    WritePrivateProfileStringA("Hotkeys", "Recenter", buf, ".\\ltr_hotkeys.ini");
    sprintf(buf, "%u", cfg_vk_pause);
    WritePrivateProfileStringA("Hotkeys", "Pause", buf, ".\\ltr_hotkeys.ini");
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

void re_register_hotkeys() {
    UnregisterHotKey(hMain, HOTKEY_ID_RECENTER);
    UnregisterHotKey(hMain, HOTKEY_ID_PAUSE);
    RegisterHotKey(hMain, HOTKEY_ID_RECENTER, 0, cfg_vk_recenter);
    RegisterHotKey(hMain, HOTKEY_ID_PAUSE, 0, cfg_vk_pause);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            hRecenterText = CreateWindowA("STATIC", "Recenter Key: F12", WS_VISIBLE | WS_CHILD, 10, 10, 200, 20, hwnd, (HMENU)IDC_ST_RECENTER, NULL, NULL);
            hRecenterBtn  = CreateWindowA("BUTTON", "Rebind", WS_VISIBLE | WS_CHILD, 220, 5, 60, 25, hwnd, (HMENU)IDC_BTN_RECENTER, NULL, NULL);
            
            hPauseText    = CreateWindowA("STATIC", "Pause Key: Pause", WS_VISIBLE | WS_CHILD, 10, 40, 200, 20, hwnd, (HMENU)IDC_ST_PAUSE, NULL, NULL);
            hPauseBtn     = CreateWindowA("BUTTON", "Rebind", WS_VISIBLE | WS_CHILD, 220, 35, 60, 25, hwnd, (HMENU)IDC_BTN_PAUSE, NULL, NULL);
            update_labels();
            re_register_hotkeys();
            break;
            
        case WM_COMMAND:
            if (LOWORD(wp) == IDC_BTN_RECENTER) {
                bind_state = STATE_BIND_RECENTER;
                update_labels();
                SetFocus(hwnd); /* Return focus to window for key capture */
            } else if (LOWORD(wp) == IDC_BTN_PAUSE) {
                bind_state = STATE_BIND_PAUSE;
                update_labels();
                SetFocus(hwnd); /* Return focus to window for key capture */
            }
            break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (bind_state != STATE_IDLE) {
                UINT vk = (UINT)wp;
                if (bind_state == STATE_BIND_RECENTER) cfg_vk_recenter = vk;
                if (bind_state == STATE_BIND_PAUSE)    cfg_vk_pause = vk;
                bind_state = STATE_IDLE;
                save_config();
                update_labels();
                re_register_hotkeys();
            }
            break;

        case WM_HOTKEY:
            if (wp == HOTKEY_ID_RECENTER) send_command("RECN");
            if (wp == HOTKEY_ID_PAUSE)    send_command("PAUS");
            break;

        case WM_DESTROY:
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
        /* Another instance is already running - exit silently */
        if (hMutex) CloseHandle(hMutex);
        return 0;
    }
    
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    
    client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset(&bridge_addr, 0, sizeof(bridge_addr));
    bridge_addr.sin_family = AF_INET;
    bridge_addr.sin_port = htons(4242);
    inet_pton(AF_INET, "127.0.0.1", &bridge_addr.sin_addr);

    load_config();

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
    
    closesocket(client_sock);
    WSACleanup();
    return 0;
}
