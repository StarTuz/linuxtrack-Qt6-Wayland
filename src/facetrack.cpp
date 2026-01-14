#include <opencv2/imgcodecs.hpp>
#include "facetrack.h"
#include "wc_driver_prefs.h"
#include <iostream>
#include <vector>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/dnn.hpp>

#include "one_euro_filter.h"
#include "utils.h"
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

static cv::Ptr<cv::FaceDetectorYN> dnn_detector;
static bool use_dnn = false;
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

static float sigmoid(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

// static int frame_size = 0;
// static int frame_w = 0; // Removed duplicate
// static int frame_h = 0; // Removed duplicate
// static uint8_t *frame = nullptr; // Removed duplicate
// static cv::Mat *cvimage; // Removed duplicate
static cv::Mat scaled;
static cv::Size minFace(40, 40);
// static float expFiltFactor = 0.2; // Removed duplicate
// static bool init = true; // Removed duplicate

// ROI logic removed as DNN handles full frame efficiently or provides its own optimization

void ltr_int_detect(cv::Mat &img) {
  if (use_dnn && dnn_detector) {
    int optim = ltr_int_wc_get_optim_level();
    cv::Mat resized;
    float scale_x = 1.0f, scale_y = 1.0f;
    
    // For DNN, we can always downscale to save CPU. 
    // Optimization levels: 0=Auto(320x320), 1=1/2, 2=1/4, 3=1/8
    // For YuNet, we prefer square-ish or proportional sizes. 
    // 320x320 is the default in many examples.
    if (optim == 0) {
      resized = img; 
    } else {
      cv::resize(img, resized, cv::Size(img.cols >> optim, img.rows >> optim));
    }
    
    scale_x = (float)img.cols / resized.cols;
    scale_y = (float)img.rows / resized.rows;
    
    cv::Mat bgr;
    cv::cvtColor(resized, bgr, cv::COLOR_GRAY2BGR);
    static cv::Size last_size(0, 0);
    if (bgr.cols != last_size.width || bgr.rows != last_size.height) {
      dnn_detector->setInputSize(bgr.size());
      last_size = bgr.size();
    }
    cv::Mat detections;
    dnn_detector->detect(bgr, detections);
    
    if (detections.rows > 0) {
      float x1 = detections.at<float>(0, 0) * scale_x;
      float y1 = detections.at<float>(0, 1) * scale_y;
      float w = detections.at<float>(0, 2) * scale_x;
      float h = detections.at<float>(0, 3) * scale_y;
      float confidence = detections.at<float>(0, 14);
      
      float conf_thresh = ltr_int_wc_get_confidence_threshold();
      if (confidence > conf_thresh) {
        missing_frames = 0;
        
        // Raw center coordinates
        float rx = (x1 + w * 0.5f) - (frame_w * 0.5f);
        float ry = (y1 + h * 0.5f) - (frame_h * 0.5f);
        
        // Calculate dt for 1-Euro filter
        auto now = std::chrono::steady_clock::now();
        float dt = 1.0f / 30.0f; // Default
        if (filters_initialized) {
          dt = std::chrono::duration<float>(now - last_frame_time).count();
        } else {
          // Repurpose "Exp-filter-factor" preference for 1-Euro Beta (0.0 to 0.1 range)
          float beta = ltr_int_wc_get_eff();
          one_euro_init(&filter_x, 0.5f, beta, 1.0f);
          one_euro_init(&filter_y, 0.5f, beta, 1.0f);
          one_euro_init(&filter_w, 0.5f, beta, 1.0f);
          one_euro_init(&filter_h, 0.5f, beta, 1.0f);
          filters_initialized = true;
        }
        last_frame_time = now;
        
        // Update params from GUI in case they changed
        float current_beta = ltr_int_wc_get_eff();
        one_euro_set_params(&filter_x, 0.5f, current_beta);
        one_euro_set_params(&filter_y, 0.5f, current_beta);
        one_euro_set_params(&filter_w, 0.5f, current_beta);
        one_euro_set_params(&filter_h, 0.5f, current_beta);

        // Apply 1-Euro Filter
        face_x = one_euro_filter(&filter_x, rx, dt);
        face_y = one_euro_filter(&filter_y, ry, dt);
        face_w = one_euro_filter(&filter_w, w, dt);
        face_h = one_euro_filter(&filter_h, h, dt);
        
        face_x1 = (x1 < 0) ? 0 : x1;
        face_y1 = (y1 < 0) ? 0 : y1;
        face_x2 = (x1 + w >= frame_w) ? frame_w - 1 : x1 + w;
        face_y2 = (y1 + h >= frame_h) ? frame_h - 1 : y1 + h;
        
        static int log_cnt = 0;
        if(++log_cnt % 60 == 0) {
            ltr_int_log_message("Face detected (smoothed): rx=%g ry=%g area=%g conf=%g dt=%f beta=%f\n", 
                                face_x, face_y, face_w * face_h, confidence, dt, current_beta);
        }
      } else {
        static int conf_log_cnt = 0;
        if(++conf_log_cnt % 30 == 0) {
            ltr_int_log_message("Face found but low confidence: %g\n", confidence);
        }
        missing_frames++;
      }
    } else {
      static int no_face_log_cnt = 0;
      if(++no_face_log_cnt % 60 == 0) {
          cv::Scalar mean = cv::mean(resized);
          ltr_int_log_message("No face candidates in DNN. Image mean brightness: %.1f\n", mean[0]);
      }
      missing_frames++;
    }
    
    if (missing_frames >= max_missing_frames) {
      face_w = face_h = 0;
      filters_initialized = false; // Reset filters on lost tracking
    }
  } else {
    // Haar Cascade support removed for modernization
  }
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
      if (!run) break;
      frame_status = PROCESSING;
    }
    
    // ltr_int_log_message("Detector thread: starting detection on new frame\n");
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
  const char *cascade_path = ltr_int_wc_get_cascade();
  if (cascade_path == nullptr) {
    ltr_int_log_message("Model path not specified!\n");
    return false;
  }
  
  std::string path(cascade_path);
  if (path.find(".onnx") != std::string::npos) {
    try {
      dnn_detector = cv::FaceDetectorYN::create(path, "", cv::Size(320, 320));
      if (dnn_detector.empty()) {
          ltr_int_log_message("Failed to create FaceDetectorYN from '%s'!\n", cascade_path);
          return false;
      }
      use_dnn = true;
      ltr_int_log_message("Loaded ONNX FaceDetectorYN model '%s'\n", cascade_path);
    } catch (const cv::Exception &e) {
      ltr_int_log_message("OpenCV exception loading ONNX model: %s\n", e.what());
      return false;
    }
  } else {
    ltr_int_log_message("Non-ONNX models are no longer supported for face tracking.\n");
    return false;
  }
  
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
  dnn_detector.release(); // Release DNN model resources
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
    blt->num_blobs = 1;
    // Axis mapping for Linuxtrack 1pt tracker (see tracking.c:update_pose_1pt)
    // Yaw uses (cx - x), Pitch uses (y - cy).
    // face_x/y already center-relative.
    // face_x positive = RIGHT, face_y positive = DOWN
    // Yaw Right => blob.x should be negative.
    // Pitch Up => blob.y should be negative? 
    // Wait, tracking.c: Pitch = blob.y - cy. 
    // If face UP, face_y is negative. We want Pitch positive.
    // So blob.y = -face_y.
    blt->blobs[0].x = -face_x; 
    blt->blobs[0].y = -face_y;
    blt->blobs[0].score = face_w * face_h;
    
    // Read smoothing from preferences
    expFiltFactor = ltr_int_wc_get_eff();
    
    ltr_int_draw_empty_square(img, face_x1, face_y1, face_x2, face_y2);
  } else {
    blt->num_blobs = 0;
  }
}

// Expfilt removed in favor of 1-Euro Filter

void ltr_int_mjpg_to_gray(unsigned char *src, size_t src_len, unsigned char *dest, int w, int h)
{
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
