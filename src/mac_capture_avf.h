#ifndef MAC_CAPTURE_AVF_H
#define MAC_CAPTURE_AVF_H

#include <stdbool.h>
#include "capture_provider.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ltr_mac_capture_avf {
  ltr_gray_capture_provider provider;
  void *internal; // Opaque pointer to AVFoundation Obj-C instance
  unsigned int width;
  unsigned int height;
} ltr_mac_capture_avf;

bool ltr_int_mac_capture_avf_init(ltr_mac_capture_avf *avf);
void ltr_int_mac_capture_avf_shutdown(ltr_mac_capture_avf *avf);
const ltr_gray_capture_provider *ltr_int_mac_capture_avf_provider(ltr_mac_capture_avf *avf);

#ifdef __cplusplus
}
#endif

#endif
