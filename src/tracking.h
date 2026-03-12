#ifndef TRACKING__H
#define TRACKING__H

#include <stdbool.h>
#include "cal.h"
#include "ltlib.h"
#include "axis.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Coding:
            bit0 (lsb) - invert camera X values
            bit1       - invert camera Y values
            bit2       - switch X and Y values (applied first!)
            bit4       - invert pitch, roll, X and Z translations (for tracking from behind)
*/
enum ORIENTATION {ORIENT_NOP = 0, ORIENT_FLIP_X = 1, ORIENT_FLIP_Y = 2, 
                   ORIENT_XCHG_XY = 4, ORIENT_FROM_BEHIND = 8};

typedef enum {
  LTR_POSE_ROUTE_SINGLE_POINT = 0,
  LTR_POSE_ROUTE_ABSOLUTE = 1,
  LTR_POSE_ROUTE_THREE_POINT = 2
} ltr_pose_route_t;


//extern linuxtrack_pose_t ltr_int_orig_pose;

bool ltr_int_init_tracking();
int ltr_int_update_pose(struct frame_type *frame);
int ltr_int_recenter_tracking();
int ltr_int_tracking_get_pose(linuxtrack_full_pose_t *pose);
bool ltr_int_postprocess_axes(ltr_axes_t axes, linuxtrack_pose_t *pose, linuxtrack_pose_t *unfiltered);
void ltr_int_rotate_camera_point(float *x, float *y, int cam_orientation);
void ltr_int_normalize_bloblist_for_camera_orientation(struct bloblist_type bl,
                                                       int cam_orientation);
ltr_pose_route_t ltr_int_select_pose_route(bool is_face,
                                           bool is_single_point,
                                           bool is_absolute,
                                           unsigned int blob_count);
void ltr_int_snapshot_frame_to_pose(const struct frame_type *frame,
                                    linuxtrack_full_pose_t *pose);
/*
double ltr_int_nonlinfilt(double x, 
              double y_minus_1,
              double filtfactor);

void ltr_int_nonlinfilt_vec(double x[3], 
              double y_minus_1[3],
              double filtfactor[3],
              double res[3]);
*/
#ifdef __cplusplus
}
#endif


#endif
