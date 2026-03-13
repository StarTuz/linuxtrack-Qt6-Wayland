#ifndef CAPTURE_PROVIDER_H
#define CAPTURE_PROVIDER_H

#include <stdbool.h>

#include "cal.h"
#include "capture_replay.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int (*next_frame)(void *ctx, ltr_gray_capture_frame *frame);
} ltr_gray_capture_provider_vtable;

typedef struct {
  const ltr_gray_capture_provider_vtable *vtable;
  void *ctx;
} ltr_gray_capture_provider;

int ltr_int_provider_get_frame(const ltr_gray_capture_provider *provider,
                               struct frame_type *frame,
                               unsigned char *preview_fallback,
                               unsigned char *tracking_fallback,
                               bool *frame_acquired);

#ifdef __cplusplus
}
#endif

#endif
