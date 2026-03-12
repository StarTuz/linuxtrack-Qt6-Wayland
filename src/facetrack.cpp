#include "facetrack.h"
#include "one_euro_filter.h"
#include "wc_driver_prefs.h"
#ifdef HAVE_ONNXRUNTIME
#include "neuralnet_tracker.h"
#endif
#include <chrono>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect/face.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "utils.h"
#include <condition_variable>
#include <math.h>
#include <mutex>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

enum detector_mode_t { DETECTOR_NONE, DETECTOR_CASCADE, DETECTOR_YUNET };

static cv::CascadeClassifier *cascade = nullptr;
static cv::Ptr<cv::FaceDetectorYN> yunet;
static detector_mode_t detector_mode = DETECTOR_NONE;
static double scale = 0.5;
static const double roi_factor = 0.3;
static const int max_missed_frames = 6;

static float face_x = 0;
static float face_y = 0;
static float face_w = 0;
static float face_h = 0;
static float face_x1 = 0;
static float face_y1 = 0;
static float face_x2 = 0;
static float face_y2 = 0;

static float last_face_x = 0.0f;
static float last_face_y = 0.0f;
static float last_face_w = 0.0f;
static float last_face_h = 0.0f;

static std::vector<cv::Rect> faces;
static cv::Rect lastCandidate;
static int missed_frames = 0;

static int frame_w = 0;
static int frame_h = 0;
static uint8_t *frame = nullptr;
static cv::Mat *cvimage = nullptr;
static cv::Mat scaled;
static cv::Mat bgr_input;
static cv::Size minFace(40, 40);
static float expFiltFactor = 0.2;
static bool init = true;

#ifdef HAVE_ONNXRUNTIME
static std::unique_ptr<ltr_neuralnet::NeuralNetTracker> nn_tracker;
static bool use_neuralnet = false;
#endif

static float pose_pitch = 0.0f;
static float pose_yaw = 0.0f;
static float pose_roll = 0.0f;
static float pose_tx = 0.0f;
static float pose_ty = 0.0f;
static float pose_tz = 0.0f;
static bool pose_valid = false;
static std::mutex pose_mutex_6dof;

#ifdef HAVE_ONNXRUNTIME
static one_euro_filter_t nn_pose_filters[6];
static bool nn_filters_initialized = false;
static std::chrono::steady_clock::time_point nn_last_frame_time;
static constexpr float nn_default_dt = 1.0f / 60.0f;
static constexpr float nn_max_dt = 0.1f;
static constexpr float nn_angle_min_cutoff = 0.8f;
static constexpr float nn_angle_beta = 0.02f;
static constexpr float nn_translation_min_cutoff = 0.6f;
static constexpr float nn_translation_beta = 0.02f;
static float nn_pose_history[6][3] = {};
static int nn_pose_history_index = 0;
static int nn_pose_history_count = 0;
#endif

float ltr_int_expfilt(float x, float y_minus_1, float filterfactor);

#ifdef HAVE_ONNXRUNTIME
static void init_nn_pose_filters() {
  for (int i = 0; i < 3; ++i) {
    one_euro_init(&nn_pose_filters[i], nn_angle_min_cutoff, nn_angle_beta,
                  1.0f);
  }
  for (int i = 3; i < 6; ++i) {
    one_euro_init(&nn_pose_filters[i], nn_translation_min_cutoff,
                  nn_translation_beta, 1.0f);
  }
  nn_filters_initialized = true;
}

static void reset_nn_pose_history() {
  nn_pose_history_index = 0;
  nn_pose_history_count = 0;
}

static float median3(float a, float b, float c) {
  if (a > b) {
    std::swap(a, b);
  }
  if (b > c) {
    std::swap(b, c);
  }
  if (a > b) {
    std::swap(a, b);
  }
  return b;
}

static float filter_nn_pose_sample(int axis, float value) {
  nn_pose_history[axis][nn_pose_history_index] = value;
  if (nn_pose_history_count < 3) {
    ++nn_pose_history_count;
  }
  if (nn_pose_history_count == 1) {
    return nn_pose_history[axis][0];
  }
  if (nn_pose_history_count == 2) {
    return 0.5f * (nn_pose_history[axis][0] + nn_pose_history[axis][1]);
  }
  return median3(nn_pose_history[axis][0], nn_pose_history[axis][1],
                 nn_pose_history[axis][2]);
}

static void advance_nn_pose_history() {
  nn_pose_history_index = (nn_pose_history_index + 1) % 3;
}
#endif

static void find_faces_with_cascade(cv::Mat &img, float factor) {
  cv::Size s(lastCandidate.width * roi_factor,
             lastCandidate.height * roi_factor);
  cv::Rect new_roi(lastCandidate.x - s.width, lastCandidate.y - s.height,
                   lastCandidate.width + 2 * s.width,
                   lastCandidate.height + 2 * s.height);
  new_roi &= cv::Rect(0, 0, img.cols, img.rows);
  cv::Mat roi(img, new_roi);
  faces.clear();
  if ((new_roi.width > 0) && (new_roi.height > 0)) {
    cascade->detectMultiScale(roi, faces, factor, 2, 0, minFace);
  }
  if (faces.size() == 0) {
    cascade->detectMultiScale(img, faces, factor, 2, 0, minFace);
  } else {
    for (std::vector<cv::Rect>::iterator i = faces.begin(); i != faces.end();
         ++i) {
      i->x += new_roi.x;
      i->y += new_roi.y;
    }
  }
}

static bool detect_face_with_yunet(cv::Mat &img, cv::Rect &candidate_rect) {
  if (yunet.empty()) {
    return false;
  }
  cv::cvtColor(img, bgr_input, cv::COLOR_GRAY2BGR);
  yunet->setInputSize(cv::Size(bgr_input.cols, bgr_input.rows));

  cv::Mat detections;
  yunet->detect(bgr_input, detections);
  if (detections.rows <= 0) {
    return false;
  }

  float best_score = -1.0f;
  for (int row = 0; row < detections.rows; ++row) {
    float score = detections.at<float>(row, 14);
    if (score > best_score) {
      best_score = score;
      candidate_rect = cv::Rect(cvRound(detections.at<float>(row, 0)),
                                cvRound(detections.at<float>(row, 1)),
                                cvRound(detections.at<float>(row, 2)),
                                cvRound(detections.at<float>(row, 3)));
    }
  }
  candidate_rect &= cv::Rect(0, 0, img.cols, img.rows);
  return (candidate_rect.width > 0) && (candidate_rect.height > 0);
}

void ltr_int_detect(cv::Mat &img) {
  double current_scale = 1.0;
  cv::Rect candidate_rect;
  bool candidate_found = false;
  bool have_fallback_detector = false;
  bool holding_neural_pose = false;

#ifdef HAVE_ONNXRUNTIME
  if (use_neuralnet && nn_tracker) {
    float pitch, yaw, roll, tx, ty, tz;
    candidate_found = nn_tracker->detect(img, pitch, yaw, roll, tx, ty, tz);
    if (candidate_found) {
      auto now = std::chrono::steady_clock::now();
      float dt = nn_default_dt;
      if (!nn_filters_initialized) {
        init_nn_pose_filters();
        reset_nn_pose_history();
      } else {
        dt = std::chrono::duration<float>(now - nn_last_frame_time).count();
        if ((dt <= 0.0f) || (dt > nn_max_dt)) {
          dt = nn_default_dt;
        }
      }
      nn_last_frame_time = now;

      const float filtered_pitch = one_euro_filter(
          &nn_pose_filters[0], filter_nn_pose_sample(0, pitch), dt);
      const float filtered_yaw = one_euro_filter(
          &nn_pose_filters[1], filter_nn_pose_sample(1, yaw), dt);
      const float filtered_roll = one_euro_filter(
          &nn_pose_filters[2], filter_nn_pose_sample(2, roll), dt);
      const float filtered_tx = one_euro_filter(
          &nn_pose_filters[3], filter_nn_pose_sample(3, tx), dt);
      const float filtered_ty = one_euro_filter(
          &nn_pose_filters[4], filter_nn_pose_sample(4, ty), dt);
      const float filtered_tz = one_euro_filter(
          &nn_pose_filters[5], filter_nn_pose_sample(5, tz), dt);
      advance_nn_pose_history();
      {
        std::lock_guard<std::mutex> lock(pose_mutex_6dof);
        pose_pitch = filtered_pitch;
        pose_yaw = filtered_yaw;
        pose_roll = filtered_roll;
        pose_tx = filtered_tx;
        pose_ty = filtered_ty;
        pose_tz = filtered_tz;
        pose_valid = true;
      }
      missed_frames = 0;
      auto roi = nn_tracker->last_face_box();
      if (roi) {
        candidate_rect = cv::Rect(cvRound(roi->x), cvRound(roi->y),
                                  cvRound(roi->width), cvRound(roi->height));
      }
    } else {
      if ((missed_frames < max_missed_frames) && (lastCandidate.width > 0) &&
          (lastCandidate.height > 0)) {
        candidate_rect = lastCandidate;
        candidate_found = true;
        holding_neural_pose = true;
        ++missed_frames;
      } else {
        std::lock_guard<std::mutex> lock(pose_mutex_6dof);
        pose_valid = false;
        nn_filters_initialized = false;
        reset_nn_pose_history();
      }
    }
  }
#endif

  have_fallback_detector = (detector_mode == DETECTOR_YUNET) ||
                           ((detector_mode == DETECTOR_CASCADE) &&
                            (cascade != nullptr));

  if (!candidate_found && detector_mode == DETECTOR_YUNET) {
    candidate_found = detect_face_with_yunet(img, candidate_rect);
  } else if (!candidate_found && have_fallback_detector) {
    switch (ltr_int_wc_get_optim_level()) {
    case 0:
      cv::equalizeHist(img, img);
      find_faces_with_cascade(img, 1.1);
      current_scale = 1;
      break;
    case 1:
      cv::equalizeHist(img, img);
      find_faces_with_cascade(img, 1.2);
      current_scale = 1;
      break;
    case 2:
      cv::resize(img, scaled, cv::Size(), scale, scale);
      cv::equalizeHist(scaled, scaled);
      find_faces_with_cascade(scaled, 1.1);
      current_scale = scale;
      break;
    case 3:
    default:
      cv::resize(img, scaled, cv::Size(), scale, scale);
      cv::equalizeHist(scaled, scaled);
      find_faces_with_cascade(scaled, 1.2);
      current_scale = scale;
      break;
    }

    double area = -1;
    const cv::Rect *candidate = nullptr;
    for (std::vector<cv::Rect>::const_iterator i = faces.begin();
         i != faces.end(); ++i) {
      if (i->area() > area) {
        candidate = &(*i);
        area = i->area();
      }
    }
    if (candidate != nullptr) {
      candidate_rect = *candidate;
      candidate_found = true;
    }
  }

  if (candidate_found) {
    if (!holding_neural_pose) {
      missed_frames = 0;
      lastCandidate = candidate_rect;
    }
    expFiltFactor = ltr_int_wc_get_eff();

    face_x1 = candidate_rect.x / current_scale;
    face_y1 = candidate_rect.y / current_scale;
    face_x2 = (candidate_rect.x + candidate_rect.width) / current_scale;
    face_y2 = (candidate_rect.y + candidate_rect.height) / current_scale;

    float x = (candidate_rect.x + candidate_rect.width / 2.0f) / current_scale -
              frame_w / 2.0f;
    float y =
        (candidate_rect.y + candidate_rect.height / 2.0f) / current_scale -
        frame_h / 2.0f;
    float w = candidate_rect.width / current_scale;
    float h = candidate_rect.height / current_scale;

    if (init) {
      last_face_x = face_x = x;
      last_face_y = face_y = y;
      last_face_w = face_w = w;
      last_face_h = face_h = h;
      init = false;
    } else {
      face_x = ltr_int_expfilt(x, last_face_x, expFiltFactor);
      face_y = ltr_int_expfilt(y, last_face_y, expFiltFactor);
      face_w = ltr_int_expfilt(w, last_face_w, expFiltFactor);
      face_h = ltr_int_expfilt(h, last_face_h, expFiltFactor);
      last_face_x = face_x;
      last_face_y = face_y;
      last_face_w = face_w;
      last_face_h = face_h;
    }
  } else if (++missed_frames > max_missed_frames) {
    face_w = 0;
    face_h = 0;
  }
}

static bool run = true;
static enum { READY, PROCESSING, DONE } frame_status = DONE;
static std::condition_variable frame_cv;
static std::mutex frame_mx;
static pthread_t detect_thread_handle;
static bool detector_thread_started = false;

void *ltr_int_detector_thread(void *) {
  while (run) {
    {
      std::unique_lock<std::mutex> lock(frame_mx);
      while (frame_status != READY) {
        frame_cv.wait(lock);
      }
      frame_status = PROCESSING;
    }
    if (!run) {
      break;
    }
    ltr_int_detect(*cvimage);
    {
      std::lock_guard<std::mutex> lock(frame_mx);
      frame_status = DONE;
    }
  }
  delete cascade;
  cascade = nullptr;
  yunet.release();
  detector_mode = DETECTOR_NONE;
#ifdef HAVE_ONNXRUNTIME
  nn_tracker.reset();
  use_neuralnet = false;
  nn_filters_initialized = false;
  reset_nn_pose_history();
#endif
  delete cvimage;
  cvimage = nullptr;
  bgr_input.release();
  free(frame);
  frame = nullptr;
  return nullptr;
}

bool ltr_int_init_face_detect() {
  cv::setNumThreads(1);
  bool neural_detector_active = false;
#ifdef HAVE_ONNXRUNTIME
  char *localizer_path = ltr_int_get_data_path("models/head-localizer.onnx");
  char *posenet_big =
      ltr_int_get_data_path("models/head-pose-0.4-big-int8.onnx");
  char *posenet_small =
      ltr_int_get_data_path("models/head-pose-0.4-small-f32.onnx");
  const char *posenet_path = nullptr;
  if ((localizer_path != nullptr) && (access(localizer_path, R_OK) == 0)) {
    if ((posenet_big != nullptr) && (access(posenet_big, R_OK) == 0)) {
      posenet_path = posenet_big;
    } else if ((posenet_small != nullptr) && (access(posenet_small, R_OK) == 0)) {
      posenet_path = posenet_small;
    }
  }
  if ((localizer_path != nullptr) && (posenet_path != nullptr)) {
    nn_tracker = std::make_unique<ltr_neuralnet::NeuralNetTracker>();
    if (nn_tracker->init(localizer_path, posenet_path)) {
      nn_tracker->set_fov(ltr_int_wc_get_camera_fov());
      use_neuralnet = true;
      neural_detector_active = true;
      ltr_int_log_message("Neural webcam tracker initialized.\n");
    } else {
      nn_tracker.reset();
    }
  }
  free(localizer_path);
  free(posenet_big);
  free(posenet_small);
#endif

  const char *cascade_path = ltr_int_wc_get_cascade();
  if (!neural_detector_active && cascade_path == nullptr) {
    ltr_int_log_message("Cascade path not specified!\n");
    return false;
  }

  const char *suffix = (cascade_path != nullptr) ? strrchr(cascade_path, '.') : nullptr;
  if (!neural_detector_active && (suffix != nullptr) &&
      (strcasecmp(suffix, ".onnx") == 0)) {
    try {
      yunet = cv::FaceDetectorYN::create(
          cascade_path, "", cv::Size(320, 320),
          ltr_int_wc_get_confidence_threshold(), 0.3f, 5000);
      detector_mode = DETECTOR_YUNET;
      ltr_int_log_message("Using YuNet face detector '%s' (threshold=%g)\n",
                          cascade_path, ltr_int_wc_get_confidence_threshold());
    } catch (const cv::Exception &e) {
      ltr_int_log_message("Could't load YuNet detector '%s' (%s)!\n",
                          cascade_path, e.what());
      return false;
    }
  } else if (!neural_detector_active) {
    cascade = new cv::CascadeClassifier();
    if (!cascade->load(cascade_path)) {
      ltr_int_log_message("Could't load cascade '%s'!\n", cascade_path);
      delete cascade;
      cascade = nullptr;
      return false;
    }
    detector_mode = DETECTOR_CASCADE;
    ltr_int_log_message("Using Haar cascade detector '%s'\n", cascade_path);
  }

  lastCandidate = cv::Rect(0, 0, 0, 0);
  face_w = 0;
  face_h = 0;
  missed_frames = 0;
  pose_valid = false;
  init = true;
  run = true;
  detector_thread_started =
      (pthread_create(&detect_thread_handle, nullptr, ltr_int_detector_thread,
                      nullptr) == 0);
  return detector_thread_started;
}

void ltr_int_stop_face_detect() {
  if (!detector_thread_started) {
    return;
  }
  run = false;
  {
    std::lock_guard<std::mutex> lock(frame_mx);
    frame_status = READY;
    frame_cv.notify_all();
  }
  pthread_join(detect_thread_handle, nullptr);
  detector_thread_started = false;
  ltr_int_log_message("Facetracker thread joined!\n");
  init = true;
  frame_status = DONE;
}

void ltr_int_face_detect(image_t *img, struct bloblist_type *blt) {
  if ((frame_w != img->w) || (frame_h != img->h) || (frame == nullptr)) {
    if (frame != nullptr) {
      free(frame);
    }
    if (cvimage != nullptr) {
      delete cvimage;
    }
    frame_w = img->w;
    frame_h = img->h;
    frame = (uint8_t *)malloc(frame_w * frame_h);
    cvimage = new cv::Mat(frame_h, frame_w, CV_8U, frame);
  }
  if (frame_status == DONE) {
    memcpy(frame, img->bitmap, frame_w * frame_h);
    {
      std::lock_guard<std::mutex> lock(frame_mx);
      frame_status = READY;
      frame_cv.notify_all();
    }
  }
  if (face_w * face_h > 0) {
    bool emitted_absolute = false;
#ifdef HAVE_ONNXRUNTIME
    if (use_neuralnet) {
      std::lock_guard<std::mutex> lock(pose_mutex_6dof);
      if (pose_valid) {
        blt->num_blobs = 3;
        blt->blobs[0].score = face_w * face_h;
        blt->blobs[0].y = -pose_pitch;
        blt->blobs[0].x = -pose_yaw;
        blt->blobs[1].x = pose_roll;
        blt->blobs[1].y = pose_tx;
        blt->blobs[2].x = pose_ty;
        blt->blobs[2].y = pose_tz;
        emitted_absolute = true;
      }
    }
#endif
    if (!emitted_absolute) {
      blt->num_blobs = 1;
      blt->blobs[0].x = -face_x;
      blt->blobs[0].y = -face_y;
      blt->blobs[0].score = face_w * face_h;
    }
    ltr_int_draw_empty_square(img, face_x1, face_y1, face_x2, face_y2);
  } else {
    blt->num_blobs = 0;
  }
}

float ltr_int_expfilt(float x, float y_minus_1, float filterfactor) {
  float y;

  y = y_minus_1 * (1.0 - filterfactor) + filterfactor * x;
  return y;
}
