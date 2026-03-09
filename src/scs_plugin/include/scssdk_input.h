/**
 * @file scssdk_input.h
 * @brief SCS Input SDK definitions for head tracking injection.
 *
 * This header defines the INPUT device registration API
 * available in SCS SDK 1.10+ for injecting head tracking data.
 *
 * NOTE: As of SDK 1.14, the Input API is still experimental
 * and may not be fully implemented in the Linux game binary.
 */

#ifndef SCSSDK_INPUT_H
#define SCSSDK_INPUT_H

#include "scssdk.h"

// Input event types
#define SCS_INPUT_EVENT_invalid 0
#define SCS_INPUT_EVENT_callback 1 // Register periodic callback
#define SCS_INPUT_EVENT_device_add 2
#define SCS_INPUT_EVENT_device_rem 3

// Input device types
typedef scs_u32_t scs_input_device_type_t;

#define SCS_INPUT_DEVICE_TYPE_invalid 0
#define SCS_INPUT_DEVICE_TYPE_generic 1    // Generic input device (axes)
#define SCS_INPUT_DEVICE_TYPE_semantical 2 // Semantic inputs (head, look, etc.)

// Input axis flags
#define SCS_INPUT_AXIS_FLAG_absolute 0x00000001
#define SCS_INPUT_AXIS_FLAG_relative 0x00000002
#define SCS_INPUT_AXIS_FLAG_normalized 0x00000004 // -1.0 to 1.0 range

// Input value structures
typedef struct {
  scs_string_t id;   // Unique identifier, e.g., "ltr_head"
  scs_string_t name; // Display name
  scs_input_device_type_t type;
} scs_input_device_info_t;

typedef struct {
  scs_float_t head_position_x;     // X offset in meters
  scs_float_t head_position_y;     // Y offset in meters
  scs_float_t head_position_z;     // Z offset in meters
  scs_float_t head_rotation_yaw;   // Yaw in radians
  scs_float_t head_rotation_pitch; // Pitch in radians
  scs_float_t head_rotation_roll;  // Roll in radians
} scs_input_head_state_t;

// Input callback for providing data each frame
typedef void (*scs_input_callback_t)(const scs_input_device_info_t *device,
                                     scs_input_head_state_t *state,
                                     scs_context_t context);

// Input registration function types
typedef scs_result_t (*scs_input_register_device_t)(
    const scs_input_device_info_t *device_info, scs_input_callback_t callback,
    scs_context_t context);

typedef scs_result_t (*scs_input_unregister_device_t)(scs_string_t device_id);

// Input initialization parameters
typedef struct {
  scs_u32_t method;
  scs_log_t log;
  scs_input_register_device_t register_device;
  scs_input_unregister_device_t unregister_device;
} scs_input_init_params_v100_t;

typedef scs_input_init_params_v100_t scs_input_init_params_t;

#endif // SCSSDK_INPUT_H
