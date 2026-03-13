#ifndef CAPTURE_PROCESS_H
#define CAPTURE_PROCESS_H

#include <stdbool.h>

#include "cal.h"
#include "image_process.h"

#ifdef __cplusplus
extern "C" {
#endif

int ltr_int_extract_blobs_from_gray_image(image_t *img,
                                          struct bloblist_type *bloblist,
                                          int min_blob_pixels,
                                          int max_blob_pixels,
                                          bool flip);
int ltr_int_process_gray_image(image_t *img, struct bloblist_type *bloblist,
                               int min_blob_pixels, int max_blob_pixels,
                               bool flip, bool face_tracking);

#ifdef __cplusplus
}
#endif

#endif
