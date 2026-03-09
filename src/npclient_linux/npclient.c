/**
 * @file npclient.c
 * @brief Native Linux implementation of NPClient (TrackIR) interface.
 *
 * This library provides the NaturalPoint TrackIR interface for native Linux
 * games. It connects to the Linuxtrack server and provides head tracking data
 * in the format expected by games that support TrackIR.
 *
 * Installation:
 *   Copy libNPClient.so to:
 *     ~/.local/share/TrackIR/NPClient.so
 *   OR the game's lib directory
 *
 * Games will load this library and call NP_* functions to get head pose.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "linuxtrack.h"

// Data structures matching NaturalPoint's NPClient interface
#pragma pack(push, 1)
typedef struct tir_data {
  short status;
  short frame;
  unsigned int cksum;
  float roll, pitch, yaw;
  float tx, ty, tz;
  float padding[9];
} tir_data_t;

typedef struct tir_signature {
  char DllSignature[200];
  char AppSignature[200];
} tir_signature_t;
#pragma pack(pop)

// Global state
static bool g_initialized = false;
static bool g_transmitting = false;
static unsigned short g_profile_id = 0;

// Utility: limit value to range
static float limit_num(float min, float val, float max) {
  if (val < min)
    return min;
  if (val > max)
    return max;
  return val;
}

// Checksum algorithm matching NaturalPoint protocol
static unsigned int cksum(unsigned char buf[], unsigned int size) {
  if ((size == 0) || (buf == NULL))
    return 0;

  int rounds = size >> 2;
  int rem = size % 4;
  int c = size;
  int a0, a2;

  while (rounds != 0) {
    a0 = *(short int *)buf;
    a2 = *(short int *)(buf + 2);
    buf += 4;
    c += a0;
    a2 ^= (c << 5);
    a2 <<= 11;
    c ^= a2;
    c += (c >> 11);
    --rounds;
  }

  switch (rem) {
  case 3:
    a0 = *(short int *)buf;
    a2 = *(signed char *)(buf + 2);
    c += a0;
    a2 = (a2 << 2) ^ c;
    c ^= (a2 << 16);
    a2 = (c >> 11);
    break;
  case 2:
    a2 = *(short int *)buf;
    c += a2;
    c ^= (c << 11);
    a2 = (c >> 17);
    break;
  case 1:
    a2 = *(signed char *)(buf);
    c += a2;
    c ^= (c << 10);
    a2 = (c >> 1);
    break;
  default:
    a2 = 0;
    break;
  }
  if (rem != 0) {
    c += a2;
  }

  c ^= (c << 3);
  c += (c >> 5);
  c ^= (c << 4);
  c += (c >> 17);
  c ^= (c << 25);
  c += (c >> 6);

  return (unsigned int)c;
}

// ============================================================================
// NPClient API Functions - Exported symbols
// ============================================================================

/**
 * NP_GetData - Get current head tracking data.
 * Primary function called by games every frame.
 */
int NP_GetData(tir_data_t *data) {
  if (!data)
    return 1;

  float r, p, y, tx, ty, tz;
  unsigned int frame;

  memset(data, 0, sizeof(tir_data_t));

  if (!g_initialized || !g_transmitting) {
    data->status = 1; // Not running
    return 1;
  }

  int res = linuxtrack_get_pose(&y, &p, &r, &tx, &ty, &tz, &frame);

  data->status = (linuxtrack_get_tracking_state() == RUNNING) ? 0 : 1;
  data->frame = frame & 0xFFFF;
  data->cksum = 0;

  // Convert degrees to NP units (16383 = 180 degrees)
  data->roll = r / 180.0f * 16383.0f;
  data->pitch = -p / 180.0f * 16383.0f;
  data->yaw = y / 180.0f * 16383.0f;

  // Convert mm to NP units (factor ~32.7)
  data->tx = -limit_num(-16383.0f, 32.7f * tx, 16383.0f);
  data->ty = limit_num(-16383.0f, 32.7f * ty, 16383.0f);
  data->tz = limit_num(-16383.0f, 32.7f * tz, 16383.0f);

  data->cksum = cksum((unsigned char *)data, sizeof(tir_data_t));

  return (res >= 0) ? 0 : 1;
}

/**
 * NP_GetSignature - Get DLL signature for validation.
 */
int NP_GetSignature(tir_signature_t *sig) {
  if (!sig)
    return 1;

  // Fill with placeholder signature
  memset(sig, 0, sizeof(tir_signature_t));
  snprintf(sig->DllSignature, sizeof(sig->DllSignature),
           "Linuxtrack NPClient v1.0");
  snprintf(sig->AppSignature, sizeof(sig->AppSignature), "LinuxTrack");

  return 0;
}

/**
 * NP_QueryVersion - Return NPClient version.
 */
int NP_QueryVersion(unsigned short *version) {
  if (version) {
    *version = 0x0500; // Version 5.0
  }
  return 0;
}

/**
 * NP_ReCenter - Recenter the head position.
 */
int NP_ReCenter(void) {
  if (g_initialized) {
    linuxtrack_recenter();
  }
  return 0;
}

/**
 * NP_RegisterProgramProfileID - Register a game profile.
 */
int NP_RegisterProgramProfileID(unsigned short id) {
  g_profile_id = id;

  // Initialize Linuxtrack if not already done
  if (!g_initialized) {
    linuxtrack_state_type state = linuxtrack_init(NULL); // Default profile
    if (state >= LINUXTRACK_OK) {
      g_initialized = true;
      linuxtrack_suspend(); // Start paused
    } else {
      fprintf(stderr, "[NPClient] Failed to init Linuxtrack: %s\n",
              linuxtrack_explain(state));
      return 1;
    }
  }

  return 0;
}

/**
 * NP_RegisterWindowHandle - Register window (stub).
 */
int NP_RegisterWindowHandle(void *hwnd) {
  (void)hwnd;
  return 0;
}

/**
 * NP_RequestData - Request specific data types (stub).
 */
int NP_RequestData(unsigned short req) {
  (void)req;
  return 0;
}

/**
 * NP_GetParameter / NP_SetParameter - Get/set parameters (stubs).
 */
int NP_GetParameter(int arg0, int arg1) {
  (void)arg0;
  (void)arg1;
  return 0;
}

int NP_SetParameter(int arg0, int arg1) {
  (void)arg0;
  (void)arg1;
  return 0;
}

/**
 * NP_StartDataTransmission - Start sending tracking data.
 */
int NP_StartDataTransmission(void) {
  if (g_initialized) {
    linuxtrack_wakeup();
    g_transmitting = true;
  }
  return 0;
}

/**
 * NP_StopDataTransmission - Stop sending tracking data.
 */
int NP_StopDataTransmission(void) {
  if (g_initialized) {
    linuxtrack_suspend();
    g_transmitting = false;
  }
  return 0;
}

/**
 * NP_StartCursor / NP_StopCursor - Cursor control (stubs).
 */
int NP_StartCursor(void) { return 0; }
int NP_StopCursor(void) { return 0; }

/**
 * NP_UnregisterWindowHandle - Unregister window (stub).
 */
int NP_UnregisterWindowHandle(void) { return 0; }

// Library constructor/destructor
__attribute__((constructor)) static void npclient_init(void) {
  fprintf(stderr, "[NPClient] Linuxtrack native NPClient loaded\n");
}

__attribute__((destructor)) static void npclient_fini(void) {
  if (g_initialized) {
    linuxtrack_shutdown();
    g_initialized = false;
  }
  fprintf(stderr, "[NPClient] Linuxtrack native NPClient unloaded\n");
}
