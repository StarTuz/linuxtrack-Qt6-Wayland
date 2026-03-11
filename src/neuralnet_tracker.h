/*
 * Neural Network Face Tracker for Linuxtrack
 *
 * Ported from OpenTrack's tracker-neuralnet module.
 * Original author: Michael Welter <michael@welter-4d.de>
 * License: ISC
 */

#pragma once

#ifdef HAVE_ONNXRUNTIME

#include <onnxruntime_cxx_api.h>
#include <opencv2/core.hpp>
#include <opencv2/core/quaternion.hpp>
#include <opencv2/imgproc.hpp>

#include <array>
#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace ltr_neuralnet {

class Localizer {
public:
  Localizer(Ort::MemoryInfo &allocator_info, Ort::Session &&session);
  std::pair<float, cv::Rect2f> run(const cv::Mat &frame);
  double last_inference_time_ms() const { return last_inference_time_; }

private:
  static constexpr int INPUT_W = 288;
  static constexpr int INPUT_H = 224;

  Ort::Session session_{nullptr};
  cv::Mat scaled_frame_;
  cv::Mat input_mat_;
  Ort::Value input_val_{nullptr};
  Ort::Value output_val_{nullptr};
  std::array<float, 5> results_{};
  double last_inference_time_ = 0.0;
};

class PoseEstimator {
public:
  struct Face {
    cv::Quatf rotation;
    cv::Rect2f box;
    cv::Point2f center;
    float size;
  };

  PoseEstimator(Ort::MemoryInfo &allocator_info, Ort::Session &&session);
  std::optional<Face> run(const cv::Mat &frame, const cv::Rect &roi);
  double last_inference_time_ms() const { return last_inference_time_; }

private:
  int64_t model_version_ = 0;
  Ort::Session session_{nullptr};
  mutable Ort::Allocator allocator_;

  cv::Mat scaled_frame_;
  cv::Mat input_mat_;
  std::vector<Ort::Value> input_val_;
  std::vector<std::string> input_names_;
  std::vector<const char *> input_c_names_;

  cv::Vec<float, 3> output_coord_{};
  cv::Vec<float, 4> output_quat_{};
  cv::Vec<float, 4> output_box_{};
  std::vector<Ort::Value> output_val_;
  std::vector<std::string> output_names_;
  std::vector<const char *> output_c_names_;

  double last_inference_time_ = 0.0;
};

class NeuralNetTracker {
public:
  NeuralNetTracker();
  ~NeuralNetTracker();

  bool init(const std::string &localizer_path, const std::string &posenet_path);
  bool detect(const cv::Mat &frame, float &pitch, float &yaw, float &roll,
              float &tx, float &ty, float &tz);

  double last_localizer_time_ms() const;
  double last_posenet_time_ms() const;
  void draw_gizmos(cv::Mat &frame) const;
  std::optional<cv::Rect2f> last_face_box() const { return last_roi_; }
  void set_fov(float diagonal_fov_degrees);

private:
  void quaternion_to_euler(const cv::Quatf &q, float &pitch, float &yaw,
                           float &roll);
  void image_to_world_position(const cv::Point2f &center, float face_size,
                               const cv::Size &image_size, float &tx,
                               float &ty, float &tz);
  static cv::Quatf image_to_world_quat(const cv::Quatf &q);
  static float iou(const cv::Rect2f &a, const cv::Rect2f &b);

  Ort::Env env_{nullptr};
  Ort::MemoryInfo alloc_info_{nullptr};
  std::unique_ptr<Localizer> localizer_;
  std::unique_ptr<PoseEstimator> posenet_;

  std::optional<cv::Rect2f> last_roi_;
  std::optional<cv::Rect2f> last_localizer_roi_;

  float focal_length_w_ = 1.0f;
  float focal_length_h_ = 1.0f;
  static constexpr float HEAD_SIZE_MM = 200.0f;

  float last_pitch_ = 0.0f;
  float last_yaw_ = 0.0f;
  float last_roll_ = 0.0f;
  float last_tx_ = 0.0f;
  float last_ty_ = 0.0f;
  float last_tz_ = 0.0f;
  bool last_valid_ = false;
};

inline float sigmoid(float x) { return 1.0f / (1.0f + std::exp(-x)); }

void normalize_brightness(const cv::Mat &frame, cv::Mat &out);
int find_input_intensity_quantile(const cv::Mat &frame, float percentage);

} // namespace ltr_neuralnet

#endif
