#include "frame_adapter.h"

#include <string.h>

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
