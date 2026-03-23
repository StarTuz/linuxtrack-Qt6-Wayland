#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "cal.h"
#include "frame_adapter.h"
#include "mac_capture_stub.h"
#include <com_proc.h>
#include "ipc_utils.h"
#include "wc_driver_prefs.h"
#include "utils.h"

char *args[] = {"./qt_cam", "-c", "Live! Cam Optia", "-x", "352", "-y", "288", "-f", "xxx", "-d", "/cascade",NULL};
static int width;
static int height;
static struct mmap_s mmm;

static ltr_mac_capture_stub gray_capture_stub;

static bool is_face_tracking_device(const struct camera_control_block *ccb)
{
  if(ccb == NULL){
    return false;
  }
  return (ccb->device.category == mac_webcam_ft);
}

static unsigned int helper_blob_capacity(const struct frame_type *frame)
{
  if(frame == NULL){
    return 0;
  }
  return frame->bloblist.num_blobs;
}

static bool should_require_cascade(const struct camera_control_block *ccb)
{
  return is_face_tracking_device(ccb);
}

static bool init_capture(char *prog, char *camera, int w, int h, char *fileName, char *cascade)
{
  char res_x[16];
  char res_y[16];
  snprintf(res_x, sizeof(res_x), "%d", w);
  snprintf(res_y, sizeof(res_y), "%d", h);
  if(ltr_int_mmap_file(fileName, ltr_int_get_com_size() + w * h, &mmm)){
    ltr_int_setCommand(&mmm, WAKEUP);
    args[0] = prog;
    args[2] = camera;
    args[4] = res_x;
    args[6] = res_y;
    args[8] = fileName;
    if(cascade != NULL){
      args[9] = "-d";
      args[10] = cascade;
    }else{
      args[9] = NULL;
    }
    bool isChild;
    ltr_int_fork_child(args, &isChild);
    return true;
  }else{
    return false;
  }
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
  if((ccb == NULL) || !ltr_int_getFrameFlag(&mmm)){
    return;
  }

  ltr_int_mac_capture_stub_submit_frame(&gray_capture_stub,
                                        ltr_int_getFramePtr(&mmm),
                                        width,
                                        height,
                                        MAX_BLOBS,
                                        (unsigned int)ltr_int_wc_get_threshold(),
                                        ltr_int_wc_get_min_blob(),
                                        ltr_int_wc_get_max_blob(),
                                        false,
                                        is_face_tracking_device(ccb));
}

static bool update_frame_from_gray_helper(const struct camera_control_block *ccb,
                                          struct frame_type *frame)
{
  bool helper_frame_acquired = false;
  const ltr_gray_capture_provider *provider = NULL;

  if((ccb == NULL) || (frame == NULL)){
    return false;
  }
  submit_gray_helper_frame(ccb);
  provider = ltr_int_mac_capture_stub_provider(&gray_capture_stub);
  if(ltr_int_provider_get_frame(provider, frame, frame->bitmap,
                                frame->bitmap_processed,
                                &helper_frame_acquired) == 0 &&
     helper_frame_acquired){
    ltr_int_resetFrameFlag(&mmm);
    return true;
  }
  if(ltr_int_getFrameFlag(&mmm) && (frame->bitmap != NULL)){
    memcpy(frame->bitmap, ltr_int_getFramePtr(&mmm), frame->width * frame->height);
    ltr_int_resetFrameFlag(&mmm);
  }
  return false;
}

static int update_frame_from_helper_blobs(struct frame_type *frame,
                                          bool *frame_acquired)
{
  if(!ltr_int_haveNewBlobs(&mmm)){
    return 0;
  }
  frame->bloblist.num_blobs = ltr_int_getBlobs(&mmm, frame->bloblist.blobs,
                                               helper_blob_capacity(frame));
  *frame_acquired = true;
  return 1;
}

static bool apply_helper_blob_override_if_available(struct frame_type *frame,
                                                    bool gray_frame_acquired,
                                                    bool *frame_acquired)
{
  if((frame == NULL) || (frame_acquired == NULL)){
    return false;
  }
  if(update_frame_from_helper_blobs(frame, frame_acquired) != 0){
    return true;
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
  
  char *cap_path = ltr_int_get_helper_path("qt_cam");
  char *cam_id = ltr_int_my_strdup(ltr_int_wc_get_id());
  char *capture_path = ltr_int_get_ipc_path("macwebcam_capture.mmap");
  char *cascade = NULL;
  if(should_require_cascade(ccb)){
    if(ltr_int_wc_get_cascade() == NULL){
	  ltr_int_log_message("No cascade specified!\n");
      free(cap_path);
      free(cam_id);
      free(capture_path);
	  return -1;
	}
    cascade = ltr_int_my_strdup(ltr_int_wc_get_cascade());
  }
  if((capture_path == NULL) ||
     !init_capture(cap_path, cam_id, width, height, capture_path, cascade)){
    free(cap_path);
    free(capture_path);
    free(cam_id);
    if(cascade != NULL){
      free(cascade);
    }
    return 1;
  }
  free(cap_path);
  free(capture_path);
  free(cam_id);
  if(cascade != NULL){
    free(cascade);
  }
  read_img_processing_prefs();
  ltr_int_mac_capture_stub_init(&gray_capture_stub);
  ltr_int_resetFrameFlag(&mmm);
  return 0;
}

int ltr_int_tracker_pause()
{
  ltr_int_setCommand(&mmm, SLEEP);
  return 0;
}

int ltr_int_tracker_resume()
{
  ltr_int_setCommand(&mmm, WAKEUP);
  return 0;
}

int ltr_int_tracker_close()
{
  ltr_int_setCommand(&mmm, STOP);
  ltr_int_wait_child_exit(10);
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
    if(!ltr_int_child_alive()){
      return -1;
    }
    ltr_int_usleep(5000);
  }
  return 0;
}
