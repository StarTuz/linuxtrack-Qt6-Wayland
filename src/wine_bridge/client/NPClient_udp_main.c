/*
 * NPClient_udp_main.c
 *
 * Simplified NPClient for UDP bridge - no file dependencies.
 * This version initializes UDP tracking immediately on load.
 */

#define __WINESRC__
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>

#include "NPClient_dll.h"
#include "linuxtrack.h"
#include "rest.h"
#include "wine/debug.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

WINE_DEFAULT_DEBUG_CHANNEL(NPClient);

extern void udp_log(const char *fmt, ...);

static bool initialized = false;
static bool crypted = false;
static unsigned char table[8] = {0,0,0,0,0,0,0,0};
static tir_signature_t cached_sig;
static bool sig_cached = false;
static game_desc_t cached_gd;
static bool gd_cached = false;
static unsigned short last_id = 0xFFFF;

static float limit_num(float min, float val, float max) {
  if (val < min)
    return min;
  if (val > max)
    return max;
  return val;
}

static unsigned int cksum(unsigned char buf[], unsigned int size) {
  if ((size == 0) || (buf == NULL)) {
    return 0;
  }

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

static void enhance(unsigned char buf[], unsigned int size,
                    unsigned char codetable[], unsigned int table_size) {
  unsigned int table_ptr = 0;
  unsigned char var = 0x88;
  unsigned char tmp;
  if ((size <= 0) || (table_size <= 0) || (buf == NULL) ||
      (codetable == NULL)) {
    return;
  }
  do {
    tmp = buf[--size];
    buf[size] = tmp ^ codetable[table_ptr] ^ var;
    var += size + tmp;
    ++table_ptr;
    if (table_ptr >= table_size) {
      table_ptr -= table_size;
    }
  } while (size != 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  (void)hinstDLL;
  (void)lpvReserved;
  
  switch (fdwReason) {
#ifdef DLL_WINE_PREATTACH
  case DLL_WINE_PREATTACH:
    return TRUE;
#endif
  case DLL_PROCESS_ATTACH:
    TRACE("UDP Bridge: DLL_PROCESS_ATTACH\n");
    /* Initialize tracking immediately on load */
    if (!initialized) {
      if (linuxtrack_init("Default") >= LINUXTRACK_OK) {
        initialized = true;
        udp_log("UDP Bridge: Tracking initialized! tir_data_t size: %d\n", (int)sizeof(tir_data_t));
      } else {
        udp_log("UDP Bridge: Failed to initialize tracking\n");
      }
    }
    break;
  case DLL_PROCESS_DETACH:
    TRACE("UDP Bridge: DLL_PROCESS_DETACH\n");
    if (initialized) {
      linuxtrack_shutdown();
      initialized = false;
    }
    break;
  }

  return TRUE;
}

int __stdcall NPCLIENT_NP_GetData(tir_data_t *data) {
  static int call_count = 0;
  float r, p, y, tx, ty, tz;
  unsigned int frame;
  int res = linuxtrack_get_pose(&y, &p, &r, &tx, &ty, &tz, &frame);
  
  call_count++;
  if (call_count <= 5 || call_count % 100 == 0) {
    udp_log("UDP Bridge: NP_GetData called #%d (y=%.2f p=%.2f r=%.2f)\n", call_count, y, p, r);
  }

  memset((char *)data, 0, sizeof(tir_data_t));
  data->status = (linuxtrack_get_tracking_state() == STOPPED) ? 1 : 0;
  data->frame = frame & 0xFFFF;
  data->cksum = 0;
  
  /* Scale to NaturalPoint units (180 deg -> 16383 units) */
  data->roll = r / 180.0f * 16383.0f;
  data->pitch = -p / 180.0f * 16383.0f; /* Inverted pitch for TrackIR parity */
  data->yaw = y / 180.0f * 16383.0f;

  /* Scale translation (mm -> units, approx 32.7 units per mm) */
  data->tx = -limit_num(-16383.0f, 32.7f * tx, 16383.0f);
  data->ty = limit_num(-16383.0f, 32.7f * ty, 16383.0f);
  data->tz = limit_num(-16383.0f, 32.7f * tz, 16383.0f);
  data->cksum = cksum((unsigned char *)data, sizeof(tir_data_t));

  if (call_count % 500 == 0) {
    unsigned char *ptr = (unsigned char *)data;
    char hex[200] = {0};
    for(int i=0; i<16 && i<(int)sizeof(tir_data_t); ++i) {
        sprintf(hex + strlen(hex), "%02X ", ptr[i]);
    }
    udp_log("UDP Bridge: Data Hex (first 16): %s\n", hex);
    udp_log("UDP Bridge: Fields - S:%d F:%d Ck:%08X R:%.2f P:%.2f Y:%.2f TX:%.2f TY:%.2f TZ:%.2f\n",
            data->status, data->frame, data->cksum,
            data->roll, data->pitch, data->yaw,
            data->tx, data->ty, data->tz);
  }

  if (crypted) {
    enhance((unsigned char *)data, sizeof(tir_data_t), table, sizeof(table));
  }
  return (res >= 0) ? 0 : 1;
}

int __stdcall NPCLIENT_NP_GetParameter(int arg0, int arg1) {
  udp_log("UDP Bridge: NP_GetParameter called (%d, %d)\n", arg0, arg1);
  return 0;
}

int __stdcall NPCLIENT_NP_GetSignature(tir_signature_t *sig) {
  if (!sig_cached) {
    udp_log("UDP Bridge: Filling signature cache...\n");
    if (getSomeSeriousPoetry(cached_sig.DllSignature, cached_sig.AppSignature)) {
      sig_cached = true;
      udp_log("UDP Bridge: Signature cache filled\n");
    } else {
      udp_log("UDP Bridge: Signature FAILED to load\n");
      return 1;
    }
  }
  memcpy(sig, &cached_sig, sizeof(tir_signature_t));
  return 0;
}

int __stdcall NPCLIENT_NP_QueryVersion(unsigned short *version) {
  udp_log("UDP Bridge: NP_QueryVersion called\n");
  *version = 0x0500;
  return 0;
}

int __stdcall NPCLIENT_NP_ReCenter(void) {
  udp_log("UDP Bridge: NP_ReCenter called\n");
  linuxtrack_recenter();
  return 0;
}

int __stdcall NPCLIENT_NP_RegisterProgramProfileID(unsigned short id) {
  if (gd_cached && last_id == id) {
    return 0;
  }

  udp_log("UDP Bridge: NP_RegisterProgramProfileID called with ID %u\n", id);
  if (game_data_get_desc(id, &cached_gd)) {
    udp_log("UDP Bridge: Game identified: %s (encrypted=%d)\n", cached_gd.name, cached_gd.encrypted);
    
    /* Emergency fallback for Elite Dangerous if keys are missing in gamedata.txt */
    if (id == 3475 && !cached_gd.encrypted) {
        udp_log("UDP Bridge: Applying emergency encryption keys for Elite Dangerous\n");
        cached_gd.encrypted = true;
        cached_gd.key1 = 0xA9485EEC;
        cached_gd.key2 = 0xA12E18BE;
    }
    
    crypted = cached_gd.encrypted;
    gd_cached = true;
    last_id = id;
    if (cached_gd.encrypted) {
      uint32_t k1 = cached_gd.key1;
      uint32_t k2 = cached_gd.key2;
      table[0] = (unsigned char)(k1 & 0xff);
      table[1] = (unsigned char)((k1 >> 8) & 0xff);
      table[2] = (unsigned char)((k1 >> 16) & 0xff);
      table[3] = (unsigned char)((k1 >> 24) & 0xff);
      table[4] = (unsigned char)(k2 & 0xff);
      table[5] = (unsigned char)((k2 >> 8) & 0xff);
      table[6] = (unsigned char)((k2 >> 16) & 0xff);
      table[7] = (unsigned char)((k2 >> 24) & 0xff);
    }
  } else {
    udp_log("UDP Bridge: Game ID %d unknown, using default\n", id);
    crypted = false;
    gd_cached = true;
    last_id = id;
  }

  /* UDP version: tracking is already initialized in DllMain */
  if (!initialized) {
    if (linuxtrack_init("Default") >= LINUXTRACK_OK) {
      initialized = true;
    } else {
      return 1;
    }
  }
  linuxtrack_suspend();
  return 0;
}

int __stdcall NPCLIENT_NP_RegisterWindowHandle(HWND hwnd) {
  udp_log("UDP Bridge: NP_RegisterWindowHandle called with HWND %p\n", hwnd);
  return 0;
}

int __stdcall NPCLIENT_NP_RequestData(unsigned short req) {
  udp_log("UDP Bridge: NP_RequestData called with req 0x%04X\n", req);
  return 0;
}

int __stdcall NPCLIENT_NP_SetParameter(int arg0, int arg1) {
  udp_log("UDP Bridge: NP_SetParameter called (%d, %d)\n", arg0, arg1);
  return 0;
}

int __stdcall NPCLIENT_NP_StartCursor(void) {
  return 0;
}

int __stdcall NPCLIENT_NP_StartDataTransmission(void) {
  udp_log("UDP Bridge: NP_StartDataTransmission called\n");
  linuxtrack_wakeup();
  return 0;
}

int __stdcall NPCLIENT_NP_StopCursor(void) {
  return 0;
}

int __stdcall NPCLIENT_NP_StopDataTransmission(void) {
  udp_log("UDP Bridge: NP_StopDataTransmission called\n");
  linuxtrack_suspend();
  return 0;
}

int __stdcall NPCLIENT_NP_UnregisterWindowHandle(void) {
  return 0;
}
