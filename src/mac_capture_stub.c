#include "mac_capture_stub.h"

static int mac_capture_stub_next_frame(void *ctx, ltr_gray_capture_frame *frame) {
  ltr_mac_capture_stub *stub = (ltr_mac_capture_stub *)ctx;

  if ((stub == NULL) || (frame == NULL)) {
    return -1;
  }
  if (!stub->frame_ready) {
    return 0;
  }

  *frame = stub->pending_frame;
  stub->frame_ready = false;
  return 1;
}

static const ltr_gray_capture_provider_vtable k_mac_capture_stub_vtable = {
    .next_frame = mac_capture_stub_next_frame,
};

void ltr_int_mac_capture_stub_init(ltr_mac_capture_stub *stub) {
  if (stub == NULL) {
    return;
  }
  stub->provider.vtable = &k_mac_capture_stub_vtable;
  stub->provider.ctx = stub;
  stub->pending_frame.gray_bitmap = NULL;
  stub->pending_frame.width = 0;
  stub->pending_frame.height = 0;
  stub->pending_frame.expected_blobs = 0;
  stub->pending_frame.threshold = 0;
  stub->pending_frame.min_blob_pixels = 0;
  stub->pending_frame.max_blob_pixels = 0;
  stub->pending_frame.flip = false;
  stub->pending_frame.face_tracking = false;
  stub->frame_ready = false;
}

void ltr_int_mac_capture_stub_submit_frame(
    ltr_mac_capture_stub *stub, const unsigned char *gray_bitmap,
    unsigned int width, unsigned int height, unsigned int expected_blobs,
    unsigned int threshold, int min_blob_pixels, int max_blob_pixels,
    bool flip, bool face_tracking) {
  if (stub == NULL) {
    return;
  }
  stub->pending_frame.gray_bitmap = gray_bitmap;
  stub->pending_frame.width = width;
  stub->pending_frame.height = height;
  stub->pending_frame.expected_blobs = expected_blobs;
  stub->pending_frame.threshold = threshold;
  stub->pending_frame.min_blob_pixels = min_blob_pixels;
  stub->pending_frame.max_blob_pixels = max_blob_pixels;
  stub->pending_frame.flip = flip;
  stub->pending_frame.face_tracking = face_tracking;
  stub->frame_ready = true;
}

const ltr_gray_capture_provider *
ltr_int_mac_capture_stub_provider(ltr_mac_capture_stub *stub) {
  if (stub == NULL) {
    return NULL;
  }
  return &(stub->provider);
}
