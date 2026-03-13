#include "frame_adapter.h"

#include <string.h>

static const int c_FRAME_TS_WRAP_USECS = 1024 * 1000000;

void ltr_int_prepare_capture_frame(struct frame_type *frame,
                                   unsigned int width,
                                   unsigned int height,
                                   unsigned int expected_blobs) {
  if (frame == NULL) {
    return;
  }
  frame->bloblist.num_blobs = expected_blobs;
  frame->width = width;
  frame->height = height;
  frame->camera_diag[0] = '\0';
  frame->camera_diag2[0] = '\0';
}

unsigned char *ltr_int_get_preview_buffer(struct frame_type *frame,
                                          unsigned char *fallback) {
  if ((frame != NULL) && (frame->bitmap != NULL)) {
    return frame->bitmap;
  }
  return fallback;
}

unsigned char *ltr_int_get_tracking_buffer(struct frame_type *frame,
                                           unsigned char *fallback) {
  if ((frame != NULL) && (frame->bitmap_processed != NULL)) {
    return frame->bitmap_processed;
  }
  return fallback;
}

void ltr_int_threshold_gray_frame(const unsigned char *source_buf,
                                  unsigned char *dest_buf,
                                  unsigned int pixel_count,
                                  unsigned int threshold) {
  unsigned int cntr;

  if ((source_buf == NULL) || (dest_buf == NULL)) {
    return;
  }

  for (cntr = 0; cntr < pixel_count; ++cntr) {
    if (source_buf[cntr] > threshold) {
      dest_buf[cntr] = source_buf[cntr];
    } else {
      dest_buf[cntr] = 0;
    }
  }
}

uint32_t ltr_int_frame_fourcc(const char fourcc[4]) {
  return ((uint32_t)(unsigned char)fourcc[0]) |
         ((uint32_t)(unsigned char)fourcc[1] << 8) |
         ((uint32_t)(unsigned char)fourcc[2] << 16) |
         ((uint32_t)(unsigned char)fourcc[3] << 24);
}

void ltr_int_fourcc_to_string(uint32_t fourcc, char out[5]) {
  out[0] = (char)(fourcc & 0xFF);
  out[1] = (char)((fourcc >> 8) & 0xFF);
  out[2] = (char)((fourcc >> 16) & 0xFF);
  out[3] = (char)((fourcc >> 24) & 0xFF);
  out[4] = '\0';
}

static void zero_gray_frame(unsigned char *dest_buf, unsigned int pixel_count) {
  memset(dest_buf, 0, pixel_count);
}

static int next_frame_timestamp(const ltr_frame_clock_state *state,
                                int capture_usec) {
  int next_usec = capture_usec;

  if ((state == NULL) || !state->initialized) {
    return next_usec;
  }

  if (capture_usec > state->last_usec) {
    return capture_usec;
  }

  if ((state->last_usec - capture_usec) > (c_FRAME_TS_WRAP_USECS / 2)) {
    return capture_usec;
  }

  next_usec = state->last_usec + 1;
  if (next_usec >= c_FRAME_TS_WRAP_USECS) {
    next_usec -= c_FRAME_TS_WRAP_USECS;
  }
  return next_usec;
}

bool ltr_int_convert_frame_to_gray(uint32_t fourcc, unsigned int width,
                                   unsigned int height,
                                   const unsigned char *source_buf,
                                   unsigned int bytes_used,
                                   unsigned char *dest_buf) {
  const unsigned int pixel_count = width * height;
  unsigned int cntr;
  unsigned int out = 0;

  if ((source_buf == NULL) || (dest_buf == NULL)) {
    return false;
  }

  if (fourcc == ltr_int_frame_fourcc("YUYV")) {
    for (cntr = 0; (cntr + 1) < bytes_used && out < pixel_count;
         cntr += 2, ++out) {
      dest_buf[out] = source_buf[cntr];
    }
  } else if ((fourcc == ltr_int_frame_fourcc("YU12")) ||
             (fourcc == ltr_int_frame_fourcc("YV12")) ||
             (fourcc == ltr_int_frame_fourcc("GREY"))) {
    out = (bytes_used < pixel_count) ? bytes_used : pixel_count;
    memcpy(dest_buf, source_buf, out);
  } else if ((fourcc == ltr_int_frame_fourcc("RGB3")) ||
             (fourcc == ltr_int_frame_fourcc("BGR3"))) {
    for (cntr = 0; (cntr + 2) < bytes_used && out < pixel_count;
         cntr += 3, ++out) {
      float y;
      if (fourcc == ltr_int_frame_fourcc("RGB3")) {
        y = 0.257f * ((float)source_buf[cntr]) +
            0.504f * ((float)source_buf[cntr + 1]) +
            0.098f * ((float)source_buf[cntr + 2]) + 16.0f;
      } else {
        y = 0.257f * ((float)source_buf[cntr + 2]) +
            0.504f * ((float)source_buf[cntr + 1]) +
            0.098f * ((float)source_buf[cntr]) + 16.0f;
      }
      if (y > 255.0f) {
        y = 255.0f;
      }
      dest_buf[out] = (unsigned char)y;
    }
  } else {
    zero_gray_frame(dest_buf, pixel_count);
    return false;
  }

  if (out < pixel_count) {
    memset(dest_buf + out, 0, pixel_count - out);
  }
  return true;
}

void ltr_int_reset_frame_clock(ltr_frame_clock_state *state) {
  if (state == NULL) {
    return;
  }
  state->counter = 0;
  state->last_usec = 0;
  state->initialized = false;
}

bool ltr_int_finalize_capture_frame(struct frame_type *frame,
                                    ltr_frame_clock_state *state,
                                    bool frame_acquired, int capture_usec) {
  if ((frame == NULL) || (state == NULL) || !frame_acquired) {
    return false;
  }

  frame->usec = next_frame_timestamp(state, capture_usec);
  frame->counter = ++(state->counter);
  state->last_usec = frame->usec;
  state->initialized = true;
  return true;
}
