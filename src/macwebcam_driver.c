#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "cal.h"
#include "capture_replay.h"
#include <com_proc.h>
#include "ipc_utils.h"
#include "wc_driver_prefs.h"
#include "utils.h"

char *args[] = {"./qt_cam", "-c", "Live! Cam Optia", "-x", "352", "-y", "288", "-f", "xxx", "-d", "/cascade",NULL};
static int width;
static int height;
static struct mmap_s mmm;


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

static void update_frame_from_gray_helper(const struct camera_control_block *ccb,
                                          struct frame_type *frame)
{
  ltr_gray_capture_frame input;

  if((frame == NULL) || ((frame->bitmap == NULL) && (frame->bitmap_processed == NULL))){
    return;
  }

  input.gray_bitmap = ltr_int_getFramePtr(&mmm);
  input.width = width;
  input.height = height;
  input.expected_blobs = frame->bloblist.num_blobs;
  input.threshold = (unsigned int)ltr_int_wc_get_threshold();
  input.min_blob_pixels = ltr_int_wc_get_min_blob();
  input.max_blob_pixels = ltr_int_wc_get_max_blob();
  input.flip = false;
  input.face_tracking = (ccb->device.category == mac_webcam_ft);

  if((frame->bitmap_processed != NULL) || input.face_tracking){
    if(ltr_int_replay_gray_capture_frame(&input, frame, NULL, NULL) == 0){
      return;
    }
  }

  if(frame->bitmap != NULL){
    memcpy(frame->bitmap, input.gray_bitmap, frame->width * frame->height);
  }
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
  if(ccb->device.category == mac_webcam_ft){
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
  frame->width = width;
  frame->height = height;
  read_img_processing_prefs();
  if(ltr_int_getFrameFlag(&mmm)){
    update_frame_from_gray_helper(ccb, frame);
    ltr_int_resetFrameFlag(&mmm);
  }
  if(ltr_int_haveNewBlobs(&mmm)){
    frame->bloblist.num_blobs = ltr_int_getBlobs(&mmm, frame->bloblist.blobs, frame->bloblist.num_blobs);
    *frame_acquired = true;
  }else{
    if(!ltr_int_child_alive()){
      return -1;
    }
    ltr_int_usleep(5000);
  }
  return 0;
}
