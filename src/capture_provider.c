#include "capture_provider.h"

int ltr_int_provider_get_frame(const ltr_gray_capture_provider *provider,
                               struct frame_type *frame,
                               unsigned char *preview_fallback,
                               unsigned char *tracking_fallback,
                               bool *frame_acquired) {
  ltr_gray_capture_frame gray_frame;
  int res;

  if ((provider == NULL) || (provider->vtable == NULL) ||
      (provider->vtable->next_frame == NULL) || (frame == NULL) ||
      (frame_acquired == NULL)) {
    return -1;
  }

  *frame_acquired = false;
  gray_frame.gray_bitmap = NULL;
  gray_frame.width = 0;
  gray_frame.height = 0;
  gray_frame.expected_blobs = 0;
  gray_frame.threshold = 0;
  gray_frame.min_blob_pixels = 0;
  gray_frame.max_blob_pixels = 0;
  gray_frame.flip = false;
  gray_frame.face_tracking = false;

  res = provider->vtable->next_frame(provider->ctx, &gray_frame);
  if (res <= 0) {
    return res;
  }

  res = ltr_int_replay_gray_capture_frame(&gray_frame, frame, preview_fallback,
                                          tracking_fallback);
  if (res == 0) {
    *frame_acquired = true;
  }
  return res;
}
