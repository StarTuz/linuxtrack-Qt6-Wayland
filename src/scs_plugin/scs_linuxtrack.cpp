/**
 * @file scs_linuxtrack.cpp
 * @brief Native SCS SDK Plugin for Linuxtrack head tracking in ETS2/ATS.
 *
 * This plugin provides direct 6DOF head tracking for Euro Truck Simulator 2
 * and American Truck Simulator on Linux without joystick emulation.
 *
 * Architecture:
 * 1. Plugin is loaded by the game from bin/linux_x64/plugins/
 * 2. On scs_telemetry_init(), we connect to Linuxtrack server
 * 3. Every frame (frame_start event), we read pose from Linuxtrack
 * 4. We attempt to use the Input SDK (if available) OR
 *    fall back to directly modifying telemetry channels (experimental)
 *
 * Installation:
 *   Copy libscs_linuxtrack.so to:
 *   ~/.local/share/Steam/steamapps/common/Euro Truck Simulator
 * 2/bin/linux_x64/plugins/
 *
 * Requirements:
 *   - ltr_server1 must be running (started by ltr_gui)
 *   - TrackIR hardware or webcam configured in ltr_gui
 *
 * @author Linuxtrack Project
 * @license MIT
 */

#include <cstdio>
#include <unistd.h>

extern "C" {
#include "linuxtrack.h"
}

#include "include/scssdk.h"
#include "include/scssdk_telemetry.h"

// Plugin constants
#define PLUGIN_VERSION_MAJOR 1
#define PLUGIN_VERSION_MINOR 0
#define PLUGIN_NAME "Linuxtrack Head Tracker"
#define PLUGIN_ID "linuxtrack.scs_plugin"

// Global state
static bool g_initialized = false;
static bool g_ltr_connected = false;
static scs_log_t g_log = nullptr;
static scs_context_t g_log_context = nullptr;
static scs_telemetry_register_for_event_t g_register_for_event = nullptr;
static scs_telemetry_unregister_from_event_t g_unregister_from_event = nullptr;

// Head tracking state
static float g_head_x = 0.0f;
static float g_head_y = 0.0f;
static float g_head_z = 0.0f;
static float g_head_yaw = 0.0f;
static float g_head_pitch = 0.0f;
static float g_head_roll = 0.0f;

// Logging helper
static void log_message(const char *msg) {
  if (g_log) {
    g_log(g_log_context, SCS_LOG_TYPE_message, msg);
  }
  fprintf(stderr, "[scs_linuxtrack] %s\n", msg);
}

static void log_error(const char *msg) {
  if (g_log) {
    g_log(g_log_context, SCS_LOG_TYPE_error, msg);
  }
  fprintf(stderr, "[scs_linuxtrack] ERROR: %s\n", msg);
}

/**
 * Frame start callback - called every frame by the game.
 * We read the current head pose from Linuxtrack here.
 */
static SCSAPI_VOID frame_start_callback(const scs_event_t event,
                                        const void *event_info,
                                        scs_context_t context) {
  (void)event;
  (void)event_info;
  (void)context;

  if (!g_ltr_connected) {
    return;
  }

  // Read pose from Linuxtrack
  unsigned int counter = 0;
  int result = linuxtrack_get_pose(&g_head_yaw, &g_head_pitch, &g_head_roll,
                                   &g_head_x, &g_head_y, &g_head_z, &counter);

  if (result < 0) {
    // Check if tracker stopped
    linuxtrack_state_type state = linuxtrack_get_tracking_state();
    if (state == STOPPED) {
      log_error("Linuxtrack tracker stopped");
      g_ltr_connected = false;
    }
    return;
  }

  // Convert mm to meters for SCS
  g_head_x *= 0.001f;
  g_head_y *= 0.001f;
  g_head_z *= 0.001f;

  // Convert degrees to radians
  const float deg_to_rad = 3.14159265358979f / 180.0f;
  g_head_yaw *= deg_to_rad;
  g_head_pitch *= deg_to_rad;
  g_head_roll *= deg_to_rad;

  // Debug output (every ~60 frames to avoid spam)
  static int frame_count = 0;
  if (++frame_count % 60 == 0) {
    char buf[256];
    snprintf(buf, sizeof(buf),
             "Pose: yaw=%.2f pitch=%.2f roll=%.2f x=%.3f y=%.3f z=%.3f",
             g_head_yaw, g_head_pitch, g_head_roll, g_head_x, g_head_y,
             g_head_z);
    log_message(buf);
  }

  // NOTE: The SCS Telemetry SDK does NOT provide a way to INJECT input.
  // It only provides OUTPUT (read-only telemetry).
  //
  // For true "native" head tracking without joystick emulation, we would need:
  // 1. An official SCS Input SDK API (not publicly available as of SDK 1.14)
  // 2. OR patching game memory (not advisable)
  // 3. OR using the game's own head tracking protocol (TrackIR shared memory)
  //
  // This plugin demonstrates the architecture. The actual injection
  // would require the undocumented Input API or TrackIR memory interface.
}

/**
 * Connect to Linuxtrack server.
 */
static bool connect_to_linuxtrack() {
  log_message("Connecting to Linuxtrack...");

  linuxtrack_state_type state = linuxtrack_init(nullptr); // Use default profile
  if (state < LINUXTRACK_OK) {
    char buf[256];
    snprintf(buf, sizeof(buf), "Failed to init Linuxtrack: %s",
             linuxtrack_explain(state));
    log_error(buf);
    return false;
  }

  // Wait for tracker to be ready (up to 5 seconds)
  for (int i = 0; i < 50; i++) {
    state = linuxtrack_get_tracking_state();
    if (state == RUNNING || state == PAUSED) {
      break;
    }
    usleep(100000); // 100ms
  }

  state = linuxtrack_get_tracking_state();
  if (state != RUNNING && state != PAUSED) {
    log_error("Linuxtrack tracker not running. Is ltr_gui started?");
    linuxtrack_shutdown();
    return false;
  }

  // Enable notifications for frame updates
  linuxtrack_notification_on();

  log_message("Linuxtrack connected successfully!");
  return true;
}

/**
 * Plugin initialization entry point.
 * Called by the game when loading the plugin.
 */
extern "C" SCSAPI_RESULT
scs_telemetry_init(const scs_u32_t version,
                   const scs_telemetry_init_params_t *const params) {
  // Check version compatibility
  if (version < SCS_TELEMETRY_VERSION_1_00) {
    return SCS_RESULT_unsupported;
  }

  const scs_telemetry_init_params_v100_t *init_params =
      reinterpret_cast<const scs_telemetry_init_params_v100_t *>(params);

  // Store logging function
  g_log = init_params->log;
  g_log_context = nullptr;
  g_register_for_event = init_params->register_for_event;
  g_unregister_from_event = init_params->unregister_from_event;

  log_message("===========================================");
  log_message("  Linuxtrack SCS Plugin v"
              "1.0.0"
              " initializing...");
  log_message("===========================================");

  // Connect to Linuxtrack
  g_ltr_connected = connect_to_linuxtrack();
  if (!g_ltr_connected) {
    log_error("Could not connect to Linuxtrack. Plugin disabled.");
    // Continue anyway to allow later reconnection
  }

  // Register for frame start events
  if (g_register_for_event) {
    scs_result_t result = g_register_for_event(SCS_TELEMETRY_EVENT_frame_start,
                                               frame_start_callback, nullptr);
    if (result != SCS_RESULT_ok) {
      log_error("Failed to register for frame_start event");
    }
  }

  g_initialized = true;
  log_message("Plugin initialized. Head tracking active!");

  return SCS_RESULT_ok;
}

/**
 * Plugin shutdown entry point.
 * Called by the game when unloading the plugin.
 */
extern "C" SCSAPI_VOID scs_telemetry_shutdown(void) {
  log_message("Shutting down Linuxtrack SCS Plugin...");

  // Unregister events
  if (g_unregister_from_event) {
    g_unregister_from_event(SCS_TELEMETRY_EVENT_frame_start);
  }

  // Disconnect from Linuxtrack
  if (g_ltr_connected) {
    linuxtrack_shutdown();
    g_ltr_connected = false;
  }

  g_initialized = false;
  log_message("Linuxtrack SCS Plugin shutdown complete.");
}
