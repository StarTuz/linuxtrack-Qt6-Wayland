#ifndef CAPTURE_REPLAY_H
#define CAPTURE_REPLAY_H

#include <stdbool.h>

#include "cal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  const unsigned char *gray_bitmap;
  unsigned int width;
  unsigned int height;
  unsigned int expected_blobs;
  unsigned int threshold;
  int min_blob_pixels;
  int max_blob_pixels;
  bool flip;
  bool face_tracking;
} ltr_gray_capture_frame;

int ltr_int_replay_gray_capture_frame(const ltr_gray_capture_frame *input,
                                      struct frame_type *frame,
                                      unsigned char *preview_fallback,
                                      unsigned char *tracking_fallback);

#ifdef __cplusplus
}
#endif

#endif
