#include "capture_process.h"

#ifdef OPENCV
#include "facetrack.h"
#endif

int ltr_int_extract_blobs_from_gray_image(image_t *img,
                                          struct bloblist_type *bloblist,
                                          int min_blob_pixels,
                                          int max_blob_pixels,
                                          bool flip) {
  unsigned int tmp;

  ltr_int_to_stripes(img);
  if (ltr_int_stripes_to_blobs(MAX_BLOBS, bloblist, min_blob_pixels,
                               max_blob_pixels, img) != 0) {
    return -1;
  }

  if (flip) {
    for (tmp = 0; tmp < bloblist->num_blobs; ++tmp) {
      bloblist->blobs[tmp].x *= -1;
      bloblist->blobs[tmp].y *= -1;
    }
  }

  return 0;
}

int ltr_int_process_gray_image(image_t *img, struct bloblist_type *bloblist,
                               int min_blob_pixels, int max_blob_pixels,
                               bool flip, bool face_tracking) {
  if (face_tracking) {
#ifdef OPENCV
    ltr_int_face_detect(img, bloblist);
    return 0;
#else
    (void)img;
    (void)bloblist;
    (void)min_blob_pixels;
    (void)max_blob_pixels;
    (void)flip;
    return -1;
#endif
  }

  return ltr_int_extract_blobs_from_gray_image(img, bloblist, min_blob_pixels,
                                               max_blob_pixels, flip);
}
