#ifndef FRAME_ADAPTER_H
#define FRAME_ADAPTER_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t ltr_int_frame_fourcc(const char fourcc[4]);
void ltr_int_fourcc_to_string(uint32_t fourcc, char out[5]);
bool ltr_int_convert_frame_to_gray(uint32_t fourcc, unsigned int width,
                                   unsigned int height,
                                   const unsigned char *source_buf,
                                   unsigned int bytes_used,
                                   unsigned char *dest_buf);

#ifdef __cplusplus
}
#endif

#endif
