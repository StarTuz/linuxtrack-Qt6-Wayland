#ifndef FACEDETECT__H
#define FACEDETECT__H

#ifdef __cplusplus
extern "C" {
#endif

#include "image_process.h"

bool ltr_int_init_face_detect();
void ltr_int_face_detect(image_t *img, struct bloblist_type *blt);
void ltr_int_stop_face_detect();
void ltr_int_mjpg_to_gray(unsigned char *src, size_t src_len, unsigned char *dest, int w, int h);

#ifdef __cplusplus
}
#endif

#endif
