#include "capture_replay.h"

#include <string.h>

#include "capture_process.h"
#include "frame_adapter.h"

int ltr_int_replay_gray_capture_frame(const ltr_gray_capture_frame *input,
                                      struct frame_type *frame,
                                      unsigned char *preview_fallback,
                                      unsigned char *tracking_fallback) {
  image_t img;
  unsigned char *preview_buf;
  unsigned char *processing_buf;
  const unsigned int pixel_count = input->width * input->height;

  if ((input == NULL) || (frame == NULL) || (input->gray_bitmap == NULL)) {
    return -1;
  }

  ltr_int_prepare_capture_frame(frame, input->width, input->height,
                                input->expected_blobs);
  preview_buf = ltr_int_get_preview_buffer(frame, preview_fallback);
  processing_buf = ltr_int_get_tracking_buffer(frame, tracking_fallback);
  if ((preview_buf == NULL) || (processing_buf == NULL)) {
    return -1;
  }

  memcpy(preview_buf, input->gray_bitmap, pixel_count);
  if (input->face_tracking) {
    if (processing_buf != preview_buf) {
      memcpy(processing_buf, preview_buf, pixel_count);
    }
  } else {
    ltr_int_threshold_gray_frame(preview_buf, processing_buf, pixel_count,
                                 input->threshold);
  }

  img.bitmap = processing_buf;
  img.w = (int)input->width;
  img.h = (int)input->height;
  img.ratio = 1.0f;
  return ltr_int_process_gray_image(&img, &(frame->bloblist),
                                    input->min_blob_pixels,
                                    input->max_blob_pixels, input->flip,
                                    input->face_tracking);
}
