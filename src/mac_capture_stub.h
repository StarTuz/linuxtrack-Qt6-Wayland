#ifndef MAC_CAPTURE_STUB_H
#define MAC_CAPTURE_STUB_H

#include <stdbool.h>

#include "capture_provider.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  ltr_gray_capture_provider provider;
  ltr_gray_capture_frame pending_frame;
  bool frame_ready;
} ltr_mac_capture_stub;

void ltr_int_mac_capture_stub_init(ltr_mac_capture_stub *stub);
void ltr_int_mac_capture_stub_submit_frame(
    ltr_mac_capture_stub *stub, const unsigned char *gray_bitmap,
    unsigned int width, unsigned int height, unsigned int expected_blobs,
    unsigned int threshold, int min_blob_pixels, int max_blob_pixels,
    bool flip, bool face_tracking);
const ltr_gray_capture_provider *
ltr_int_mac_capture_stub_provider(ltr_mac_capture_stub *stub);

#ifdef __cplusplus
}
#endif

#endif
