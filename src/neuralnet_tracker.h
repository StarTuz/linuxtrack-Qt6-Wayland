/*
 * Neural Network Face Tracker for Linuxtrack
 * 
 * Ported from OpenTrack's tracker-neuralnet module.
 * Original author: Michael Welter <michael@welter-4d.de>
 * License: ISC (see 3rdparty-notices/OPENTRACK-NEURALNET-COPYING.txt)
 * 
 * This implements direct 6DOF head pose estimation using ONNX models:
 * - head-localizer.onnx: Finds face bounding box
 * - head-pose-*.onnx: Estimates head pose from face crop
 */

#pragma once

#ifdef HAVE_ONNXRUNTIME

#include <onnxruntime_cxx_api.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/quaternion.hpp>
#include <optional>
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <chrono>

namespace ltr_neuralnet {

// ============================================================================
// Localizer: Finds face bounding box in frame
// ============================================================================
class Localizer {
public:
    Localizer(Ort::MemoryInfo& allocator_info, Ort::Session&& session);
    
    // Returns (confidence [0-1], ROI rect in frame coordinates)
    std::pair<float, cv::Rect2f> run(const cv::Mat& frame);
    
    double last_inference_time_ms() const { return last_inference_time_; }
    
private:
    static constexpr int INPUT_W = 288;
    static constexpr int INPUT_H = 224;
    
    Ort::Session session_{nullptr};
    cv::Mat scaled_frame_;
    cv::Mat input_mat_;
    Ort::Value input_val_{nullptr};
    Ort::Value output_val_{nullptr};
    std::array<float, 5> results_;
    double last_inference_time_ = 0;
};

// ============================================================================
// PoseEstimator: Gets 6DOF pose from face crop
// ============================================================================
class PoseEstimator {
public:
    struct Face {
        cv::Quatf rotation;         // Head rotation as quaternion
        cv::Rect2f box;             // Refined bounding box
        cv::Point2f center;         // Face center in frame coordinates
        float size;                 // Face size in pixels
    };
    
    PoseEstimator(Ort::MemoryInfo& allocator_info, Ort::Session&& session);
    
    // Run pose estimation on a face crop
    // Returns nullopt if detection fails
    std::optional<Face> run(const cv::Mat& frame, const cv::Rect& roi);
    
    double last_inference_time_ms() const { return last_inference_time_; }
    
private:
    int64_t model_version_ = 0;
    Ort::Session session_{nullptr};
    mutable Ort::Allocator allocator_;
    
    cv::Mat scaled_frame_;
    cv::Mat input_mat_;
    std::vector<Ort::Value> input_val_;
    std::vector<std::string> input_names_;
    std::vector<const char*> input_c_names_;
    
    cv::Vec<float, 3> output_coord_{};  // x, y, size
    cv::Vec<float, 4> output_quat_{};   // quaternion components
    cv::Vec<float, 4> output_box_{};    // bounding box
    std::vector<Ort::Value> output_val_;
    std::vector<std::string> output_names_;
    std::vector<const char*> output_c_names_;
    
    double last_inference_time_ = 0;
};

// ============================================================================
// NeuralNetTracker: Main tracker combining localizer + pose estimator
// ============================================================================
class NeuralNetTracker {
public:
    NeuralNetTracker();
    ~NeuralNetTracker();
    
    // Initialize with paths to ONNX model files
    bool init(const std::string& localizer_path, const std::string& posenet_path);
    
    // Run detection on a frame
    // Returns true if face detected, fills 6DOF pose values (in degrees and mm)
    bool detect(const cv::Mat& frame,
                float& pitch, float& yaw, float& roll,
                float& tx, float& ty, float& tz);
    
    // Get inference timing
    double last_localizer_time_ms() const;
    double last_posenet_time_ms() const;
    
    // Draw debugging visualization on frame
    void draw_gizmos(cv::Mat& frame) const;
    
    // Get last detected face box (for visualization)
    std::optional<cv::Rect2f> last_face_box() const { return last_roi_; }
    
    // Camera intrinsics (should be set based on actual camera)
    void set_fov(float diagonal_fov_degrees);
    
private:
    // Convert image-space quaternion to world-space Euler angles
    void quaternion_to_euler(const cv::Quatf& q, float& pitch, float& yaw, float& roll);
    
    // Convert image position to world translation
    void image_to_world_position(const cv::Point2f& center, float face_size,
                                  const cv::Size& image_size,
                                  float& tx, float& ty, float& tz);
    
    // Coordinate space conversions (matches OpenTrack conventions)
    static cv::Quatf image_to_world_quat(const cv::Quatf& q);
    
    // IOU (intersection over union) for ROI matching
    static float iou(const cv::Rect2f& a, const cv::Rect2f& b);
    
    Ort::Env env_{nullptr};
    Ort::MemoryInfo alloc_info_{nullptr};
    std::unique_ptr<Localizer> localizer_;
    std::unique_ptr<PoseEstimator> posenet_;
    
    // ROI tracking between frames
    std::optional<cv::Rect2f> last_roi_;
    std::optional<cv::Rect2f> last_localizer_roi_;
    
    // Camera intrinsics
    float focal_length_w_ = 1.0f;
    float focal_length_h_ = 1.0f;
    static constexpr float HEAD_SIZE_MM = 200.0f;  // Approximate vertical head size
    
    // Last detected pose (for visualization)
    float last_pitch_ = 0, last_yaw_ = 0, last_roll_ = 0;
    float last_tx_ = 0, last_ty_ = 0, last_tz_ = 0;
    bool last_valid_ = false;
};

// ============================================================================
// Utility functions
// ============================================================================

// Sigmoid activation
inline float sigmoid(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

// Automatic brightness normalization
void normalize_brightness(const cv::Mat& frame, cv::Mat& out);

// Find intensity quantile for brightness normalization
int find_input_intensity_quantile(const cv::Mat& frame, float percentage);

} // namespace ltr_neuralnet

#endif // HAVE_ONNXRUNTIME
