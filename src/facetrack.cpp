#include "facetrack.h"
#include "wc_driver_prefs.h"
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <vector>

#include <opencv2/calib3d.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "one_euro_filter.h"
#include "utils.h"
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Neural Network Face Tracker (OpenTrack port)
#ifdef HAVE_ONNXRUNTIME
#include "neuralnet_tracker.h"
static std::unique_ptr<ltr_neuralnet::NeuralNetTracker> nn_tracker;
static bool use_neuralnet = false;
#endif

// static cv::Ptr<cv::FaceDetectorYN> dnn_detector;
// static bool use_dnn = false;
static double scale = 0.5;
static const double roi_factor = 0.3;
static cv::Mat *cvimage = nullptr;
static uint8_t *frame = nullptr;
static int frame_w = 0;
static int frame_h = 0;
static float face_x = -1.0;
static float face_y = -1.0;
static float face_w = 0.0;
static float face_h = 0.0;
static float face_x1 = 0.0;
static float face_y1 = 0.0;
static float face_x2 = 0.0;
static float face_y2 = 0.0;
static float expFiltFactor = 0.1;
static int missing_frames = 0;
static const int max_missing_frames = 5;

// One Euro Filter instances for smoothed face tracking
static one_euro_filter_t filter_x, filter_y, filter_w, filter_h;
static std::chrono::steady_clock::time_point last_frame_time;
static bool filters_initialized = false;
static bool lm_filters_initialized = false;
static bool pose_filters_initialized = false;
static one_euro_filter_t landmark_filters[10];
static one_euro_filter_t pose_filters[6];

// Neural net specific filters (separate from YuNet path)
#ifdef HAVE_ONNXRUNTIME
static one_euro_filter_t nn_pose_filters[6]; // Pitch, Yaw, Roll, Tx, Ty, Tz
static bool nn_filters_initialized = false;
static std::chrono::steady_clock::time_point nn_last_frame_time;
#endif

// 6DOF data exchange globals
static float pose_pitch = 0.0f;
static float pose_yaw = 0.0f;
static float pose_roll = 0.0f;
static float pose_tx = 0.0f;
static float pose_ty = 0.0f;
static float pose_tz = 0.0f;
static bool pose_valid = false;
static std::mutex pose_mutex_6dof;

// Persistent PnP guess state
static cv::Mat pnp_rvec = cv::Mat::zeros(3, 1, CV_64F);
static cv::Mat pnp_tvec = cv::Mat::zeros(3, 1, CV_64F);
static bool pnp_has_guess = false;

// Visual Marker Globals (Absolute Image Coordinates)
static float vis_face_x1 = 0, vis_face_y1 = 0, vis_face_x2 = 0, vis_face_y2 = 0;
static float vis_lm_x[5], vis_lm_y[5];
static bool vis_valid = false;

static float sigmoid(float x) { return 1.0f / (1.0f + std::exp(-x)); }

// static int frame_size = 0;
// static int frame_w = 0; // Removed duplicate
// static int frame_h = 0; // Removed duplicate
// static uint8_t *frame = nullptr; // Removed duplicate
// static cv::Mat *cvimage; // Removed duplicate
static cv::Mat scaled;
static cv::Size minFace(40, 40);
// static float expFiltFactor = 0.2; // Removed duplicate
// static bool init = true; // Removed duplicate

// ROI logic removed as DNN handles full frame efficiently or provides its own
// optimization

void ltr_int_detect(cv::Mat &img) {
  // Neural Net Tracker (OpenTrack port) - Direct 6DOF pose estimation
#ifdef HAVE_ONNXRUNTIME
  if (use_neuralnet && nn_tracker) {
    float pitch, yaw, roll, tx, ty, tz;
    bool detected = nn_tracker->detect(img, pitch, yaw, roll, tx, ty, tz);

    if (detected) {
      // Calculate dt for One Euro filter
      auto now = std::chrono::steady_clock::now();
      float dt = 1.0f / 30.0f; // Default

      if (!nn_filters_initialized) {
        // Initialize filters with tuned parameters for neural net output
        // Neural net output is already relatively stable, so use gentle
        // filtering mincutoff=1.0 (Hz), beta=0.007 (low jitter reduction),
        // dcutoff=1.0
        for (int i = 0; i < 6; ++i) {
          one_euro_init(&nn_pose_filters[i], 1.0, 0.007, 1.0);
        }
        nn_filters_initialized = true;
        ltr_int_log_message("Neural net One Euro filters initialized\n");
      } else {
        dt = std::chrono::duration<float>(now - nn_last_frame_time).count();
      }
      nn_last_frame_time = now;

      // Apply One Euro filtering to smooth the neural net output
      float f_pitch = one_euro_filter(&nn_pose_filters[0], pitch, dt);
      float f_yaw = one_euro_filter(&nn_pose_filters[1], yaw, dt);
      float f_roll = one_euro_filter(&nn_pose_filters[2], roll, dt);
      float f_tx = one_euro_filter(&nn_pose_filters[3], tx, dt);
      float f_ty = one_euro_filter(&nn_pose_filters[4], ty, dt);
      float f_tz = one_euro_filter(&nn_pose_filters[5], tz, dt);

      {
        std::lock_guard<std::mutex> lock(pose_mutex_6dof);
        pose_pitch = f_pitch;
        pose_yaw = f_yaw;
        pose_roll = f_roll;
        pose_tx = f_tx;
        pose_ty = f_ty;
        pose_tz = f_tz;
        pose_valid = true;
      }

      // For visualization compatibility, set face dimensions
      auto roi = nn_tracker->last_face_box();
      if (roi) {
        face_w = roi->width;
        face_h = roi->height;
        face_x = roi->x + roi->width / 2 - img.cols / 2;
        face_y = roi->y + roi->height / 2 - img.rows / 2;
        face_x1 = roi->x;
        face_y1 = roi->y;
        face_x2 = roi->x + roi->width;
        face_y2 = roi->y + roi->height;
        vis_face_x1 = face_x1;
        vis_face_y1 = face_y1;
        vis_face_x2 = face_x2;
        vis_face_y2 = face_y2;
        vis_valid = true;
      }

      static int nn_log = 0;
      if (++nn_log % 60 == 0) {
        ltr_int_log_message("NeuralNet 6DOF (filtered): P=%.1f Y=%.1f R=%.1f "
                            "TX=%.1f TY=%.1f TZ=%.1f dt=%.3f\n",
                            f_pitch, f_yaw, f_roll, f_tx, f_ty, f_tz, dt);
      }

      missing_frames = 0;
    } else {
      pose_valid = false;
      vis_valid = false;
      missing_frames++;

      if (missing_frames >= max_missing_frames) {
        face_w = face_h = 0;
        nn_filters_initialized = false; // Reset filters on lost tracking
      }
    }
    return; // Skip YuNet+PnP path
  }
#endif

  // YuNet Fallback Removed
  /* YuNet Fallback Removed */
}

static bool run = true;
static enum { READY, PROCESSING, DONE } frame_status = DONE;
static std::mutex frame_mx;
static std::condition_variable frame_cv;
static pthread_t detect_thread_handle;

void *ltr_int_detector_thread(void *) {
  while (run) {
    {
      std::unique_lock<std::mutex> lock(frame_mx);
      frame_cv.wait(lock, [] { return frame_status == READY || !run; });
      if (!run)
        break;
      frame_status = PROCESSING;
    }

    // ltr_int_log_message("Detector thread: starting detection on new
    // frame\n");
    double t = (double)cv::getTickCount();
    ltr_int_detect(*cvimage);
    t = (double)cv::getTickCount() - t;
    // std::cout<<"detection time = "<<t/((double)cvGetTickFrequency()*1000.)<<"
    // ms\n";

    {
      std::lock_guard<std::mutex> lock(frame_mx);
      frame_status = DONE;
    }
  }
  // Cleanup moved to ltr_int_stop_face_detect
  return nullptr;
}

bool ltr_int_init_face_detect() {
  ltr_int_log_message("ltr_int_init_face_detect() starting...\n");
  cv::setNumThreads(0); // Use all available threads

#ifdef HAVE_ONNXRUNTIME
  // Try to initialize Neural Net Tracker (OpenTrack port) first
  // This provides better 6DOF tracking than YuNet+PnP
  {
    std::string data_dir(LTR_DATA_PATH);
    std::string localizer_path;
    std::string posenet_path;

    // Search order: models/ subdirectory first, then direct share path
    std::vector<std::string> search_paths = {data_dir + "/models", data_dir};

    for (const auto &base : search_paths) {
      std::string loc = base + "/head-localizer.onnx";
      if (access(loc.c_str(), R_OK) == 0) {
        localizer_path = loc;
        // Try posenet models in order of preference
        std::vector<std::string> posenet_names = {
            "/head-pose-0.4-big-int8.onnx", "/head-pose-0.4-small-f32.onnx"};
        for (const auto &pn : posenet_names) {
          std::string ppath = base + pn;
          if (access(ppath.c_str(), R_OK) == 0) {
            posenet_path = ppath;
            break;
          }
        }
        if (!posenet_path.empty())
          break;
      }
    }

    // Check if both models were found
    if (!localizer_path.empty() && !posenet_path.empty()) {
      nn_tracker = std::make_unique<ltr_neuralnet::NeuralNetTracker>();
      if (nn_tracker->init(localizer_path, posenet_path)) {
        // Get camera FOV from preferences if available (default ~80 degrees
        // diagonal)
        nn_tracker->set_fov(80.0f);
        use_neuralnet = true;
        ltr_int_log_message(
            "Neural net tracker initialized successfully (OpenTrack port)\n");
        ltr_int_log_message("  Localizer: %s\n", localizer_path.c_str());
        ltr_int_log_message("  PoseNet: %s\n", posenet_path.c_str());

        run = true;
        return pthread_create(&detect_thread_handle, nullptr,
                              ltr_int_detector_thread, nullptr) == 0;
      } else {
        ltr_int_log_message(
            "Failed to initialize neural net tracker, falling back to YuNet\n");
        nn_tracker.reset();
      }
    } else {
      ltr_int_log_message(
          "Neural net models not found in %s, falling back to YuNet\n",
          data_dir.c_str());
      ltr_int_log_message("  (Checked: %s/models/ and %s/)\n", data_dir.c_str(),
                          data_dir.c_str());
    }
  }
#endif

  // YuNet Fallback Removed
  ltr_int_log_message(
      "Legacy YuNet tracker removed. Please use neural net models.\n");
  run = true;
  return pthread_create(&detect_thread_handle, nullptr, ltr_int_detector_thread,
                        nullptr) == 0;
}

void ltr_int_stop_face_detect() {
  run = false;
  {
    std::lock_guard<std::mutex> lock(frame_mx);
    frame_status = READY; // Wake up the thread to see 'run = false'
    frame_cv.notify_all();
  }
  pthread_join(detect_thread_handle, nullptr);
  ltr_int_log_message("Facetracker thread joined!\n");

#ifdef HAVE_ONNXRUNTIME
  if (nn_tracker) {
    nn_tracker.reset();
    use_neuralnet = false;
    ltr_int_log_message("Neural net tracker released\n");
  }
#endif

  // dnn_detector.release(); // Removed

  if (cvimage != nullptr) {
    delete cvimage;
    cvimage = nullptr;
  }
  if (frame != nullptr) {
    free(frame);
    frame = nullptr;
  }
  // init = true; // No longer used
  frame_status = DONE;
  filters_initialized = false; // Ensure filters are reset for next init
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
    blt->num_blobs = 3;
    blt->blobs[0].score = face_w * face_h;

    // Transport via blobs (Absolute Mode Layout):
    {
      std::lock_guard<std::mutex> lock(pose_mutex_6dof);
      // Compensate for tracking.c update_pose_1pt scaling (val * 200 / width)
      float s = (frame_w > 0) ? ((float)frame_w / 200.0f) : 3.2f;

      blt->blobs[0].y =
          -pose_pitch * s; // Input - -> Tracking - (Correct Pitch)
      blt->blobs[0].x =
          pose_yaw * s; // Input + -> Tracking - (Correct Yaw via invert)
      blt->blobs[1].x = pose_roll; // Z-Rotation+ is already TiltLeft+

      // Translations:
      blt->blobs[1].y = pose_tx;
      blt->blobs[2].x = pose_ty;
      blt->blobs[2].y = pose_tz;

      // Bridge debug - One-line summary
      static int bridge_log = 0;
      if (++bridge_log % 60 == 0) {
        ltr_int_log_message("Bridge 6DOF: Valid=%d Pitch=%.1f Yaw=%.1f\n",
                            pose_valid, pose_pitch, pose_yaw);
      }
    }

    // Read smoothing from preferences
    expFiltFactor = ltr_int_wc_get_eff();

    if (vis_valid) {
      static int vis_log = 0;
      if (++vis_log % 60 == 0) {
        ltr_int_log_message(
            "Vis Status: Box(%.1f,%.1f) P/Y/R=(%.1f,%.1f,%.1f) Valid=%d\n",
            vis_face_x1, vis_face_y1, pose_pitch, pose_yaw, pose_roll,
            pose_valid);
      }
      ltr_int_draw_empty_square(img, (int)vis_face_x1, (int)vis_face_y1,
                                (int)vis_face_x2, (int)vis_face_y2);
      for (int i = 0; i < 5; ++i) {
        ltr_int_draw_square(img, (int)vis_lm_x[i], (int)vis_lm_y[i], 3);
      }
    }
  } else {
    blt->num_blobs = 0;
  }
}

// Expfilt removed in favor of 1-Euro Filter

void ltr_int_mjpg_to_gray(unsigned char *src, size_t src_len,
                          unsigned char *dest, int w, int h) {
  try {
    cv::Mat rawData(1, src_len, CV_8UC1, src);
    cv::Mat decoded = cv::imdecode(rawData, cv::IMREAD_GRAYSCALE);
    if (decoded.empty()) {
      return;
    }

    if (decoded.cols != w || decoded.rows != h) {
      cv::resize(decoded, decoded, cv::Size(w, h));
    }

    // Copy to destination (assuming dest is pre-allocated w*h)
    size_t copy_size = w * h;
    if (decoded.total() < copy_size) {
      copy_size = decoded.total();
    }
    memcpy(dest, decoded.data, copy_size);
  } catch (...) {
    // Ignore decoding errors
  }
}
