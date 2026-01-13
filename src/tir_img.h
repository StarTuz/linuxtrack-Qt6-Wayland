#ifndef TIR_IMG__H
#define TIR_IMG__H

#include "list.h"
#include "image_process.h"
#include "tir_hw.h"

int ltr_int_read_blobs_tir(struct bloblist_type *blt, int min, int max, image_t *img, tir_info *info);

// Signal that an external USB reset occurred (called by ltr_int_reset_usb_tir)
// Tracking loop will reset its internal state when it sees this
void ltr_int_signal_usb_reset_occurred(void);

#endif
