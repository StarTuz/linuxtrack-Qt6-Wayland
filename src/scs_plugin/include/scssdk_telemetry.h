/**
 * @file scssdk_telemetry.h
 * @brief SCS Telemetry SDK definitions.
 *
 * Minimal subset for Linuxtrack head-tracking plugin.
 */

#ifndef SCSSDK_TELEMETRY_H
#define SCSSDK_TELEMETRY_H

#include "scssdk.h"

// Telemetry version
#define SCS_TELEMETRY_VERSION_1_00 SCS_MAKE_VERSION(1, 0)
#define SCS_TELEMETRY_VERSION_1_01 SCS_MAKE_VERSION(1, 1)
#define SCS_MAKE_VERSION(major, minor) (((major) << 16) | (minor))

#define SCS_TELEMETRY_VERSION_CURRENT SCS_TELEMETRY_VERSION_1_01

// Event types
typedef scs_u32_t scs_event_t;

#define SCS_TELEMETRY_EVENT_invalid 0
#define SCS_TELEMETRY_EVENT_frame_start 1
#define SCS_TELEMETRY_EVENT_frame_end 2
#define SCS_TELEMETRY_EVENT_paused 3
#define SCS_TELEMETRY_EVENT_started 4
#define SCS_TELEMETRY_EVENT_configuration 5
#define SCS_TELEMETRY_EVENT_gameplay 6

// Event callback
typedef SCSAPI_VOID (*scs_telemetry_frame_start_t)(const scs_event_t event,
                                                   const void *event_info,
                                                   scs_context_t context);
typedef SCSAPI_VOID (*scs_telemetry_frame_end_t)(const scs_event_t event,
                                                 const void *event_info,
                                                 scs_context_t context);
typedef SCSAPI_VOID (*scs_telemetry_paused_t)(const scs_event_t event,
                                              const void *event_info,
                                              scs_context_t context);
typedef SCSAPI_VOID (*scs_telemetry_started_t)(const scs_event_t event,
                                               const void *event_info,
                                               scs_context_t context);

// Registration function types
typedef SCSAPI_RESULT (*scs_telemetry_register_for_event_t)(
    scs_event_t event, scs_telemetry_frame_start_t callback,
    scs_context_t context);

typedef SCSAPI_RESULT (*scs_telemetry_unregister_from_event_t)(
    scs_event_t event);

// Channel callback type (must be defined before register function)
typedef SCSAPI_VOID (*scs_telemetry_channel_callback_t)(
    scs_string_t name, scs_u32_t index, const scs_value_t *value,
    scs_context_t context);

// Channel registration
typedef SCSAPI_RESULT (*scs_telemetry_register_for_channel_t)(
    scs_string_t name, scs_u32_t index, scs_value_type_t type, scs_u32_t flags,
    scs_telemetry_channel_callback_t callback, scs_context_t context);

// Common parameters
typedef struct {
  scs_u32_t method;
  scs_log_t log;
  scs_telemetry_register_for_event_t register_for_event;
  scs_telemetry_unregister_from_event_t unregister_from_event;
  scs_telemetry_register_for_channel_t register_for_channel;
} scs_telemetry_init_params_v100_t;

typedef scs_telemetry_init_params_v100_t scs_telemetry_init_params_t;

// Telemetry channels for head position (these are OUTPUT channels, not input!)
#define SCS_TELEMETRY_TRUCK_CHANNEL_head_offset "truck.head.offset"
#define SCS_TELEMETRY_TRUCK_CHANNEL_cabin_offset "truck.cabin.offset"
#define SCS_TELEMETRY_TRUCK_CHANNEL_cabin_angular_velocity                     \
  "truck.cabin.angular.velocity"

#endif // SCSSDK_TELEMETRY_H
