/**
 * @file scssdk.h
 * @brief Common SCS SDK types and definitions.
 *
 * This is a minimal subset of the official SCS SDK headers needed
 * for the Linuxtrack head-tracking plugin.
 *
 * Official SDK: https://modding.scssoft.com/wiki/Documentation/SDK
 */

#ifndef SCSSDK_H
#define SCSSDK_H

#include <stddef.h>

// Platform detection
#ifdef _WIN32
#define SCS_ARCHITECTURE_x64 1
#define WINAPI __stdcall
#define SCSAPI_RESULT int WINAPI
#define SCSAPI_VOID void WINAPI
#else
#define SCSAPI_RESULT int
#define SCSAPI_VOID void
#endif

// Basic types
typedef char scs_char_t;
typedef signed char scs_s8_t;
typedef unsigned char scs_u8_t;
typedef short scs_s16_t;
typedef unsigned short scs_u16_t;
typedef int scs_s32_t;
typedef unsigned int scs_u32_t;
typedef long long scs_s64_t;
typedef unsigned long long scs_u64_t;
typedef float scs_float_t;
typedef double scs_double_t;

typedef scs_u32_t scs_result_t;
typedef scs_u32_t scs_timestamp_t;

#define SCS_RESULT_ok 0
#define SCS_RESULT_unsupported (-1)
#define SCS_RESULT_invalid_parameter (-2)
#define SCS_RESULT_already_registered (-3)
#define SCS_RESULT_not_found (-4)
#define SCS_RESULT_unsupported_type (-5)
#define SCS_RESULT_not_now (-6)
#define SCS_RESULT_generic_error (-7)

typedef void *scs_context_t;
typedef scs_u64_t scs_token_t;

// String constants
typedef const scs_char_t *scs_string_t;

// Value types
typedef scs_u32_t scs_value_type_t;

#define SCS_VALUE_TYPE_INVALID 0
#define SCS_VALUE_TYPE_bool 1
#define SCS_VALUE_TYPE_s32 2
#define SCS_VALUE_TYPE_u32 3
#define SCS_VALUE_TYPE_u64 4
#define SCS_VALUE_TYPE_float 5
#define SCS_VALUE_TYPE_double 6
#define SCS_VALUE_TYPE_fvector 7
#define SCS_VALUE_TYPE_dvector 8
#define SCS_VALUE_TYPE_euler 9
#define SCS_VALUE_TYPE_fplacement 10
#define SCS_VALUE_TYPE_dplacement 11
#define SCS_VALUE_TYPE_string 12

// Vector and placement types
typedef struct {
  scs_float_t x;
  scs_float_t y;
  scs_float_t z;
} scs_value_fvector_t;

typedef struct {
  scs_double_t x;
  scs_double_t y;
  scs_double_t z;
} scs_value_dvector_t;

typedef struct {
  scs_float_t heading; // Rotation around Y axis (yaw)
  scs_float_t pitch;   // Rotation around X axis
  scs_float_t roll;    // Rotation around Z axis
} scs_value_euler_t;

typedef struct {
  scs_value_fvector_t position;
  scs_value_euler_t orientation;
} scs_value_fplacement_t;

typedef struct {
  scs_value_dvector_t position;
  scs_value_euler_t orientation;
} scs_value_dplacement_t;

// Named value
typedef union {
  scs_u32_t value_bool;
  scs_s32_t value_s32;
  scs_u32_t value_u32;
  scs_u64_t value_u64;
  scs_float_t value_float;
  scs_double_t value_double;
  scs_value_fvector_t value_fvector;
  scs_value_dvector_t value_dvector;
  scs_value_euler_t value_euler;
  scs_value_fplacement_t value_fplacement;
  scs_value_dplacement_t value_dplacement;
  scs_string_t value_string;
} scs_value_t;

typedef struct {
  scs_string_t name;
  scs_u32_t index;
  scs_value_type_t type;
  scs_value_t value;
} scs_named_value_t;

// Log types
typedef scs_u32_t scs_log_type_t;

#define SCS_LOG_TYPE_message 0
#define SCS_LOG_TYPE_warning 1
#define SCS_LOG_TYPE_error 2

// Log function type
typedef void (*scs_log_t)(scs_context_t context, scs_log_type_t type,
                          const scs_char_t *message);

#endif // SCSSDK_H
