/*
 * udp_bridge_test.c
 * 
 * Simple test to verify the UDP bridge DLL loads and receives data.
 * Build with: winegcc -m64 -o udp_bridge_test.exe.so udp_bridge_test.c
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

/* TrackIR SDK types - must match NPClient_dll.h exactly */
#pragma pack(push, 1)
typedef struct {
    short frame;
    short status;
    unsigned int cksum;
    float roll;
    float pitch;
    float yaw;
    float tx;
    float ty;
    float tz;
} tir_data_t;
#pragma pack(pop)

/* Function pointers for NPClient.dll */
typedef int (__stdcall *NP_GetData_t)(tir_data_t *data);
typedef int (__stdcall *NP_QueryVersion_t)(unsigned short *version);
typedef int (__stdcall *NP_StartDataTransmission_t)(void);
typedef int (__stdcall *NP_StopDataTransmission_t)(void);

int main(int argc, char *argv[]) {
    HMODULE dll;
    NP_GetData_t NP_GetData;
    NP_QueryVersion_t NP_QueryVersion;
    NP_StartDataTransmission_t NP_StartDataTransmission;
    tir_data_t data;
    unsigned short version;
    int i;
    
    (void)argc;
    (void)argv;
    
    printf("UDP Bridge Test\n");
    printf("===============\n\n");
    
    /* Try to load NPClient64.dll from same directory */
    printf("Loading NPClient64.dll...\n");
    fflush(stdout);
    
    dll = LoadLibraryA("NPClient64.dll");
    if (!dll) {
        printf("Failed to load NPClient64.dll (error %lu), trying NPClient.dll...\n", GetLastError());
        fflush(stdout);
        dll = LoadLibraryA("NPClient.dll");
    }
    
    if (!dll) {
        printf("ERROR: Could not load NPClient DLL! Error: %lu\n", GetLastError());
        return 1;
    }
    
    printf("DLL loaded successfully!\n\n");
    fflush(stdout);
    
    /* Get function pointers - just the simple ones */
    NP_QueryVersion = (NP_QueryVersion_t)GetProcAddress(dll, "NP_QueryVersion");
    NP_StartDataTransmission = (NP_StartDataTransmission_t)GetProcAddress(dll, "NP_StartDataTransmission");
    NP_GetData = (NP_GetData_t)GetProcAddress(dll, "NP_GetData");
    
    printf("NP_QueryVersion: %p\n", (void*)NP_QueryVersion);
    printf("NP_StartDataTransmission: %p\n", (void*)NP_StartDataTransmission);
    printf("NP_GetData: %p\n", (void*)NP_GetData);
    fflush(stdout);
    
    if (!NP_GetData || !NP_QueryVersion) {
        printf("ERROR: Could not get function pointers!\n");
        FreeLibrary(dll);
        return 1;
    }
    
    /* Query version - simple, safe call */
    printf("\nQuerying version...\n");
    fflush(stdout);
    version = 0;
    NP_QueryVersion(&version);
    printf("Version: 0x%04X\n", version);
    fflush(stdout);
    
    /* Try to start transmission (this initializes tracking in UDP version) */
    if (NP_StartDataTransmission) {
        printf("Starting data transmission...\n");
        fflush(stdout);
        NP_StartDataTransmission();
    }
    
    printf("\nReading tracking data (5 samples):\n");
    fflush(stdout);
    
    for (i = 0; i < 5; i++) {
        memset(&data, 0, sizeof(data));
        int result = NP_GetData(&data);
        
        printf("Frame %d: Y=%.1f P=%.1f R=%.1f (result=%d)\n",
               data.frame, data.yaw, data.pitch, data.roll, result);
        fflush(stdout);
        
        Sleep(500);
    }
    
    printf("\nTest complete.\n");
    FreeLibrary(dll);
    
    return 0;
}

