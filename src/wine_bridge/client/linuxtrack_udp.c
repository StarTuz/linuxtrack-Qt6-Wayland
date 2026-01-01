/*
 * linuxtrack_udp.c
 *
 * UDP-based implementation of linuxtrack.h API for Wine/Proton.
 * Receives pose data from ltr_udp on the Linux host via UDP.
 *
 * This replaces the dlopen-based approach which is brittle in
 * containerized environments (Flatpak, Proton sandbox, etc.)
 *
 * Configuration:
 *   LTR_UDP_HOST - IP to listen on (default: 127.0.0.1)
 *   LTR_UDP_PORT - Port to listen on (default: 4242)
 */

/* Windows headers MUST come first to avoid conflicts */
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

/* Now safe to include standard C headers */
#include "linuxtrack.h"
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Default configuration - bind to all interfaces to receive from Linux host */
#define DEFAULT_HOST "0.0.0.0"
#define DEFAULT_PORT 4242

/* OpenTrack UDP packet format: 6 doubles */
typedef struct __attribute__((packed)) {
    double x;     /* cm */
    double y;     /* cm */
    double z;     /* cm */
    double yaw;   /* degrees */
    double pitch; /* degrees */
    double roll;  /* degrees */
} opentrack_udp_t;

/* Internal state */
static SOCKET udp_socket = INVALID_SOCKET;
static HANDLE receiver_thread = NULL;
static volatile bool running = false;
static CRITICAL_SECTION pose_mutex;
static bool mutex_initialized = false;

/* Current pose data */
static float current_yaw = 0.0f;
static float current_pitch = 0.0f;
static float current_roll = 0.0f;
static float current_tx = 0.0f;
static float current_ty = 0.0f;
static float current_tz = 0.0f;
static uint32_t frame_counter = 0;

/* Recenter offsets (client-side implementation) */
static float recenter_yaw = 0.0f;
static float recenter_pitch = 0.0f;
static float recenter_roll = 0.0f;
static float recenter_tx = 0.0f;
static float recenter_ty = 0.0f;
static float recenter_tz = 0.0f;

/* Tracking state */
static linuxtrack_state_type tracking_state = STOPPED;
static bool paused = false;

/* Debug logging - always enabled for now to help diagnose issues */
static FILE *log_file = NULL;
static bool log_initialized = false;

void udp_log(const char *fmt, ...) {
    if (!log_initialized) {
        log_initialized = true;
        /* Always log to help debug - remove this check once stable */
        log_file = fopen("C:\\ltr_udp_client.log", "a");
        if (!log_file) {
            /* Try current directory as fallback */
            log_file = fopen("ltr_udp_client.log", "a");
        }
    }
    if (log_file) {
        va_list ap;
        va_start(ap, fmt);
        vfprintf(log_file, fmt, ap);
        fflush(log_file);
        va_end(ap);
    }
}

/* Receiver thread function */
static DWORD WINAPI receiver_thread_func(LPVOID param) {
    (void)param;
    char buffer[100];
    struct sockaddr_in sender_addr;
    int sender_len = sizeof(sender_addr);
    int timeout_count = 0;
    int packet_count = 0;
    int loop_count = 0;
    
    udp_log("Receiver thread started, waiting for UDP packets on port 4242...\n");
    
    while (running) {
        loop_count++;
        int bytes = recvfrom(udp_socket, buffer, sizeof(buffer), 0,
                             (struct sockaddr*)&sender_addr, &sender_len);
        
        if (bytes == sizeof(opentrack_udp_t)) {
            opentrack_udp_t *packet = (opentrack_udp_t *)buffer;
            packet_count++;
            if (packet_count <= 5 || packet_count % 500 == 0) {
                udp_log("Received pose #%d: yaw=%.2f pitch=%.2f roll=%.2f\n",
                        packet_count, packet->yaw, packet->pitch, packet->roll);
            }
            
            EnterCriticalSection(&pose_mutex);
            current_tx = (float)(packet->x * 10.0);
            current_ty = (float)(packet->y * 10.0);
            current_tz = (float)(packet->z * 10.0);
            current_yaw = (float)packet->yaw;
            current_pitch = (float)packet->pitch;
            current_roll = (float)packet->roll;
            frame_counter++;
            
            if (tracking_state != RUNNING && !paused) {
                tracking_state = RUNNING;
                udp_log("Tracking state changed to RUNNING\n");
            }
            LeaveCriticalSection(&pose_mutex);
        } else if (bytes == 4) {
             /* Handle commands from ltr_udp server */
             if (memcmp(buffer, "RECN", 4) == 0) {
                 /* Server has recentered - reset our client-side offsets to zero.
                  * This is important: server sends already-recentered data after RECN,
                  * so we must NOT add more offset on top.
                  */
                 udp_log("UDP Bridge: Received RECENTER command - resetting client offsets\n");
                 EnterCriticalSection(&pose_mutex);
                 recenter_yaw = 0.0f;
                 recenter_pitch = 0.0f;
                 recenter_roll = 0.0f;
                 recenter_tx = 0.0f;
                 recenter_ty = 0.0f;
                 recenter_tz = 0.0f;
                 LeaveCriticalSection(&pose_mutex);
             } else if (memcmp(buffer, "PAUS", 4) == 0) {
                 udp_log("UDP Bridge: Received PAUSE command (currently paused=%d)\n", paused);
                 if (paused) {
                     linuxtrack_wakeup();
                     udp_log("UDP Bridge: Tracking resumed\n");
                 } else {
                     linuxtrack_suspend();
                     udp_log("UDP Bridge: Tracking paused\n");
                 }
             }
        } else if (bytes == SOCKET_ERROR) {
            int err = WSAGetLastError();
            if (err == WSAETIMEDOUT) {
                timeout_count++;
            } else if (err != WSAEWOULDBLOCK) {
                udp_log("recvfrom error: %d\n", err);
            }
        }
    }
    
    udp_log("Receiver thread exiting...\n");
    return 0;
}

static void auto_launch_hotkeys(void) {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    char *cmd = "ltr_wine_hotkeys.exe";
    
    /* 
     * Try to find ltr_wine_hotkeys.exe in the same directory as the game 
     * or Program Files\Linuxtrack
     */
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    udp_log("UDP Bridge: Attempting to launch hotkey utility...\n");
    
    /* 1. Try Linuxtrack directory */
    char hotkey_path[MAX_PATH];
    if (GetEnvironmentVariableA("ProgramFiles", hotkey_path, MAX_PATH)) {
        strcat(hotkey_path, "\\Linuxtrack\\ltr_wine_hotkeys.exe");
        if (CreateProcessA(hotkey_path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            udp_log("UDP Bridge: Launched hotkeys from %s\n", hotkey_path);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return;
        }
    }
    
    /* 2. Try current directory as fallback */
    if (CreateProcessA(cmd, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        udp_log("UDP Bridge: Launched hotkeys from current directory\n");
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return;
    }
    
    udp_log("UDP Bridge: Could not find or launch ltr_wine_hotkeys.exe\n");
}

linuxtrack_state_type linuxtrack_init(const char *cust_section) {
    (void)cust_section;
    WSADATA wsa_data;
    struct sockaddr_in bind_addr;
    const char *host_env;
    const char *port_env;
    const char *host = DEFAULT_HOST;
    int port = DEFAULT_PORT;
    DWORD timeout = 100; /* 100ms receive timeout */
    
    udp_log("linuxtrack_init called\n");
    
    /* Check environment variables */
    host_env = getenv("LTR_UDP_HOST");
    if (host_env && host_env[0]) {
        host = host_env;
    }
    
    port_env = getenv("LTR_UDP_PORT");
    if (port_env && port_env[0]) {
        port = atoi(port_env);
        if (port <= 0 || port > 65535) {
            port = DEFAULT_PORT;
        }
    }
    
    udp_log("Configured: host=%s port=%d\n", host, port);
    
    /* Initialize Winsock */
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        udp_log("WSAStartup failed\n");
        return err_NOT_INITIALIZED;
    }
    
    /* Initialize mutex */
    if (!mutex_initialized) {
        InitializeCriticalSection(&pose_mutex);
        mutex_initialized = true;
    }
    
    /* Create UDP socket */
    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket == INVALID_SOCKET) {
        udp_log("socket() failed: %d\n", WSAGetLastError());
        WSACleanup();
        return err_NOT_INITIALIZED;
    }
    
    /* Set receive timeout so thread can check running flag */
    setsockopt(udp_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    
    /* Allow address reuse */
    int reuse = 1;
    setsockopt(udp_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
    
    /* Bind to configured address */
    memset(&bind_addr, 0, sizeof(bind_addr));
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons((u_short)port);
    inet_pton(AF_INET, host, &bind_addr.sin_addr);
    
    if (bind(udp_socket, (struct sockaddr*)&bind_addr, sizeof(bind_addr)) == SOCKET_ERROR) {
        udp_log("bind() failed: %d\n", WSAGetLastError());
        closesocket(udp_socket);
        udp_socket = INVALID_SOCKET;
        WSACleanup();
        return err_NOT_INITIALIZED;
    }
    
    udp_log("Socket bound to %s:%d\n", host, port);
    
    /* Start receiver thread */
    running = true;
    tracking_state = INITIALIZING;
    receiver_thread = CreateThread(NULL, 0, receiver_thread_func, NULL, 0, NULL);
    if (receiver_thread == NULL) {
        udp_log("CreateThread failed\n");
        running = false;
        closesocket(udp_socket);
        udp_socket = INVALID_SOCKET;
        WSACleanup();
        return err_NOT_INITIALIZED;
    }
    
    udp_log("Initialization complete\n");
    auto_launch_hotkeys();
    return LINUXTRACK_OK;
}

linuxtrack_state_type linuxtrack_shutdown(void) {
    udp_log("linuxtrack_shutdown called\n");
    
    /* Signal thread to stop */
    running = false;
    
    /* Wait for thread to exit */
    if (receiver_thread != NULL) {
        WaitForSingleObject(receiver_thread, 1000);
        CloseHandle(receiver_thread);
        receiver_thread = NULL;
    }
    
    /* Close socket */
    if (udp_socket != INVALID_SOCKET) {
        closesocket(udp_socket);
        udp_socket = INVALID_SOCKET;
    }
    
    /* Cleanup Winsock */
    WSACleanup();
    
    /* Cleanup mutex */
    if (mutex_initialized) {
        DeleteCriticalSection(&pose_mutex);
        mutex_initialized = false;
    }
    
    tracking_state = STOPPED;
    
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
    
    return LINUXTRACK_OK;
}

linuxtrack_state_type linuxtrack_suspend(void) {
    paused = true;
    tracking_state = PAUSED;
    return LINUXTRACK_OK;
}

linuxtrack_state_type linuxtrack_wakeup(void) {
    paused = false;
    if (frame_counter > 0) {
        tracking_state = RUNNING;
    }
    return LINUXTRACK_OK;
}

linuxtrack_state_type linuxtrack_recenter(void) {
    EnterCriticalSection(&pose_mutex);
    
    /* Store current pose as recenter offset */
    recenter_yaw = current_yaw;
    recenter_pitch = current_pitch;
    recenter_roll = current_roll;
    recenter_tx = current_tx;
    recenter_ty = current_ty;
    recenter_tz = current_tz;
    
    LeaveCriticalSection(&pose_mutex);
    
    udp_log("Recentered at yaw=%.2f pitch=%.2f roll=%.2f\n",
            recenter_yaw, recenter_pitch, recenter_roll);
    
    return LINUXTRACK_OK;
}

int linuxtrack_get_pose(float *heading, float *pitch, float *roll,
                        float *tx, float *ty, float *tz, uint32_t *counter) {
    static uint32_t last_counter = 0;
    int new_data = 0;
    
    if (paused) {
        *heading = *pitch = *roll = *tx = *ty = *tz = 0.0f;
        *counter = frame_counter;
        return 0;
    }
    
    EnterCriticalSection(&pose_mutex);
    
    /* Apply client-side recenter offset.
     * This provides in-game recentering capability via NP_ReCenter().
     * The offsets are set to 0 initially and only change when
     * linuxtrack_recenter() is explicitly called.
     */
    *heading = current_yaw - recenter_yaw;
    *pitch = current_pitch - recenter_pitch;
    *roll = current_roll - recenter_roll;
    *tx = current_tx - recenter_tx;
    *ty = current_ty - recenter_ty;
    *tz = current_tz - recenter_tz;
    *counter = frame_counter;
    
    if (frame_counter != last_counter) {
        new_data = 1;
        last_counter = frame_counter;
    }
    
    LeaveCriticalSection(&pose_mutex);
    
    return new_data;
}

int linuxtrack_get_abs_pose(float *heading, float *pitch, float *roll,
                            float *tx, float *ty, float *tz, uint32_t *counter) {
    /* Absolute pose ignores recenter offset */
    static uint32_t last_counter = 0;
    int new_data = 0;
    
    EnterCriticalSection(&pose_mutex);
    
    *heading = current_yaw;
    *pitch = current_pitch;
    *roll = current_roll;
    *tx = current_tx;
    *ty = current_ty;
    *tz = current_tz;
    *counter = frame_counter;
    
    if (frame_counter != last_counter) {
        new_data = 1;
        last_counter = frame_counter;
    }
    
    LeaveCriticalSection(&pose_mutex);
    
    return new_data;
}

int linuxtrack_get_pose_full(linuxtrack_pose_t *pose, float blobs[],
                             int num_blobs, int *blobs_read) {
    int new_data;
    uint32_t counter;
    
    new_data = linuxtrack_get_pose(&pose->yaw, &pose->pitch, &pose->roll,
                                   &pose->tx, &pose->ty, &pose->tz, &counter);
    
    /* We don't have raw pose data via UDP, so use the same values */
    pose->raw_yaw = pose->yaw;
    pose->raw_pitch = pose->pitch;
    pose->raw_roll = pose->roll;
    pose->raw_tx = pose->tx;
    pose->raw_ty = pose->ty;
    pose->raw_tz = pose->tz;
    
    /* No blob data available via UDP */
    *blobs_read = 0;
    (void)blobs;
    (void)num_blobs;
    
    return new_data;
}

linuxtrack_state_type linuxtrack_get_tracking_state(void) {
    return tracking_state;
}

const char *linuxtrack_explain(linuxtrack_state_type status) {
    switch (status) {
    case INITIALIZING:
        return "Linuxtrack UDP client is initializing.";
    case RUNNING:
        return "Linuxtrack UDP client is running.";
    case PAUSED:
        return "Linuxtrack UDP client is paused.";
    case STOPPED:
        return "Linuxtrack UDP client is stopped.";
    case err_NOT_INITIALIZED:
        return "Linuxtrack UDP client is not initialized.";
    case err_NOT_FOUND:
        return "Linuxtrack UDP: No data received. Is ltr_udp running?";
    default:
        return "Unknown state.";
    }
}

/* Stubs for functions not applicable to UDP mode */
linuxtrack_state_type linuxtrack_request_frames(void) {
    return err_NOT_INITIALIZED; /* Not supported via UDP */
}

int linuxtrack_get_frame(int *req_width, int *req_height, size_t buf_size,
                         uint8_t *buffer) {
    (void)req_width;
    (void)req_height;
    (void)buf_size;
    (void)buffer;
    return err_NOT_INITIALIZED; /* Not supported via UDP */
}

linuxtrack_state_type linuxtrack_notification_on(void) {
    return LINUXTRACK_OK; /* No-op for UDP */
}

int linuxtrack_get_notify_pipe(void) {
    return -1; /* Not applicable */
}

int linuxtrack_wait(int timeout) {
    Sleep(timeout);
    return 0;
}
