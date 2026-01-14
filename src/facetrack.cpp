#include <opencv2/imgcodecs.hpp>
#include "facetrack.h"
#include "wc_driver_prefs.h"
#include <iostream>
#include <vector>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/dnn.hpp>

// ... existing code ...



#include "utils.h"
#include <condition_variable>
#include <math.h>
#include <mutex>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

static cv::CascadeClassifier *cascade = nullptr;
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
static float last_face_x = 0.0;
static float last_face_y = 0.0;
static float last_face_w = 0.0;
static float last_face_h = 0.0;
static cv::Rect lastCandidate(0, 0, 0, 0);
static bool init = true;
static float expFiltFactor = 0.1;
static int missing_frames = 0;
static const int max_missing_frames = 5;
static const float confidence_threshold = 0.45f;

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

float ltr_int_expfilt(float x, float y_minus_1, float filterfactor);

void ltr_int_find_faces(cv::Mat &img, float factor) {
  cv::Size s(lastCandidate.width * roi_factor,
             lastCandidate.height * roi_factor);
  cv::Rect new_roi(lastCandidate.x - s.width, lastCandidate.y - s.height,
                   lastCandidate.width + 2 * s.width,
                   lastCandidate.height + 2 * s.height);
  new_roi &= cv::Rect(0, 0, img.cols, img.rows);
  cv::Mat roi(img, new_roi);
  // faces.clear(); // faces is now local to ltr_int_detect for Haar
  // if ((new_roi.width > 0) && (new_roi.height > 0)) {
  //   cascade->detectMultiScale(roi, faces, factor, 2, 0, minFace);
  // }
  // if (faces.size() == 0) {
  //   cascade->detectMultiScale(img, faces, factor, 2, 0, minFace);
  // } else {
  //   for (std::vector<cv::Rect>::iterator i = faces.begin(); i != faces.end();
  //        ++i) {
  //     i->x += new_roi.x;
  //     i->y += new_roi.y;
  //   }
  // }
}

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
      cv::resize(img, resized, cv::Size(320, 320));
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
      
      if (confidence > confidence_threshold) {
        missing_frames = 0;
        float x = (x1 + w * 0.5f) - (frame_w * 0.5f);
        float y = (y1 + h * 0.5f) - (frame_h * 0.5f);
        
        face_x = x;
        face_y = y;
        face_w = w;
        face_h = h;
        face_x1 = x1;
        face_y1 = y1;
        face_x2 = x1 + w;
        face_y2 = y1 + h;
      } else {
        missing_frames++;
      }
    } else {
      missing_frames++;
    }
    
    if (missing_frames >= max_missing_frames) {
      face_w = face_h = 0;
    }
  } else if (cascade != nullptr) {
    std::vector<cv::Rect> faces;
    int optim = ltr_int_wc_get_optim_level();
    cv::Mat resized;
    if (optim > 0) {
      cv::resize(img, resized, cv::Size(img.cols >> optim, img.rows >> optim));
    } else {
      resized = img;
    }
    cascade->detectMultiScale(resized, faces, 1.2, 3,
                              cv::CASCADE_FIND_BIGGEST_OBJECT |
                                  cv::CASCADE_DO_ROUGH_SEARCH,
                              cv::Size(resized.cols / 4, resized.rows / 4));
    if (faces.size() > 0) {
      float x = (faces[0].x + faces[0].width / 2) << optim;
      float y = (faces[0].y + faces[0].height / 2) << optim;
      float w = faces[0].width << optim;
      float h = faces[0].height << optim;
      
      face_x1 = faces[0].x << optim;
      face_y1 = faces[0].y << optim;
      face_x2 = (faces[0].x + faces[0].width) << optim;
      face_y2 = (faces[0].y + faces[0].height) << optim;

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
    } else {
      face_w = face_h = 0;
    }
  }
}

static bool run = true;
static enum { READY, PROCESSING, DONE } frame_status = DONE;
// static bool request_frame = false;
static std::condition_variable frame_cv;
static std::mutex frame_mx;
static pthread_t detect_thread_handle;

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
  delete cascade;
  cascade = nullptr;
  delete cvimage;
  cvimage = nullptr;
  free(frame);
  frame = nullptr;
  return nullptr;
}

bool ltr_int_init_face_detect() {
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
    cascade = new cv::CascadeClassifier();
    if (!cascade->load(cascade_path)) {
      ltr_int_log_message("Couldn't load cascade '%s'!\n", cascade_path);
      delete cascade;
      cascade = nullptr;
      return false;
    }
    use_dnn = false;
    ltr_int_log_message("Loaded Haar cascade '%s'\n", cascade_path);
  }
  
  lastCandidate = cv::Rect(0, 0, 0, 0);
  run = true;
  return pthread_create(&detect_thread_handle, nullptr, ltr_int_detector_thread,
                        nullptr) == 0;
}

void ltr_int_stop_face_detect() {
  run = false;
  {
    std::lock_guard<std::mutex> lock(frame_mx);
    frame_status = READY;
    frame_cv.notify_all();
  }
  pthread_join(detect_thread_handle, nullptr);
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

float ltr_int_expfilt(float x, float y_minus_1, float filterfactor) {
  float y;

  y = y_minus_1 * (1.0 - filterfactor) + filterfactor * x;
  return y;
}

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
