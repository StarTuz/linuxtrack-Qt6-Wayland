#include <unistd.h>
#include <stdbool.h>
#include "cal.h"
#include "frame_adapter.h"
#include "mac_capture_avf.h"
#include "com_proc.h"
#include "ipc_utils.h"
#include "wc_driver_prefs.h"
#include "utils.h"

// We no longer fork qt_cam natively if we use AVFoundation
static ltr_mac_capture_avf gray_capture_avf;
static int width;
static int height;
static struct mmap_s mmm;

static unsigned int helper_blob_capacity(const struct frame_type *frame)
{
  if(frame == NULL){
    return 0;
  }
  return frame->bloblist.num_blobs;
}


static bool read_img_processing_prefs()
{
  ltr_int_setThreshold(&mmm, ltr_int_wc_get_threshold());
  ltr_int_setMinBlob(&mmm, ltr_int_wc_get_min_blob());
  ltr_int_setMaxBlob(&mmm, ltr_int_wc_get_max_blob());
  ltr_int_setOptLevel(&mmm, ltr_int_wc_get_optim_level());
  ltr_int_setEff(&mmm, ltr_int_wc_get_eff());
  return true;
}

static void submit_gray_helper_frame(const struct camera_control_block *ccb)
{
  // With native AVFoundation, we don't push frames to AVFoundation; it pushes to us.
  (void)ccb;
}

static bool update_frame_from_gray_helper(const struct camera_control_block *ccb,
                                          struct frame_type *frame)
{
  bool helper_frame_acquired = false;
  const ltr_gray_capture_provider *provider = NULL;

  if((ccb == NULL) || (frame == NULL)){
    return false;
  }
  
  provider = ltr_int_mac_capture_avf_provider(&gray_capture_avf);
  if(ltr_int_provider_get_frame(provider, frame, frame->bitmap,
                                frame->bitmap_processed,
                                &helper_frame_acquired) == 0 &&
     helper_frame_acquired){
    return true;
  }
  
  return false;
}

static int update_frame_from_helper_blobs(struct frame_type *frame,
                                          bool *frame_acquired)
{
  (void)frame;
  (void)frame_acquired;
  return 0; // AVFoundation directly gives pixels, not blobs currently
}

static bool apply_helper_blob_override_if_available(struct frame_type *frame,
                                                    bool gray_frame_acquired,
                                                    bool *frame_acquired)
{
  if((frame == NULL) || (frame_acquired == NULL)){
    return false;
  }
  if(gray_frame_acquired){
    *frame_acquired = true;
    return true;
  }
  return false;
}

int ltr_int_tracker_init(struct camera_control_block *ccb)
{
  if(!ltr_int_wc_init_prefs()){
    return -1;
  }
  
  ltr_int_wc_get_resolution(&width, &height);
  
  read_img_processing_prefs();
  
  if(!ltr_int_mac_capture_avf_init(&gray_capture_avf)){
    ltr_int_log_message("Failed to initialize AVFoundation capture\n");
    return -1;
  }
  
  return 0;
}

int ltr_int_tracker_pause()
{
  // AVF Pause not specifically needed beyond ignoring frames
  return 0;
}

int ltr_int_tracker_resume()
{
  return 0;
}

int ltr_int_tracker_close()
{
  ltr_int_mac_capture_avf_shutdown(&gray_capture_avf);
  ltr_int_wc_close_prefs();
  return 0;
}

int ltr_int_tracker_get_frame(struct camera_control_block *ccb, 
			      struct frame_type *frame, bool *frame_acquired)
{
  const unsigned int blob_capacity = helper_blob_capacity(frame);
  bool gray_frame_acquired = false;

  ltr_int_prepare_capture_frame(frame, width, height, blob_capacity);
  read_img_processing_prefs();
  gray_frame_acquired = update_frame_from_gray_helper(ccb, frame);
  if(!apply_helper_blob_override_if_available(frame, gray_frame_acquired,
                                              frame_acquired)){
    ltr_int_usleep(5000);
  }
  return 0;
}
