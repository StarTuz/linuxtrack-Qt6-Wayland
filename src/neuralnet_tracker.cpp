/*
 * Neural Network Face Tracker for Linuxtrack
 * 
 * Ported from OpenTrack's tracker-neuralnet module.
 * Original author: Michael Welter <michael@welter-4d.de>
 * License: ISC (see 3rdparty-notices/OPENTRACK-NEURALNET-COPYING.txt)
 */

#ifdef HAVE_ONNXRUNTIME

#include "neuralnet_tracker.h"
#include "utils.h"

#include <opencv2/imgproc.hpp>
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace ltr_neuralnet {

// ============================================================================
// Utility functions
// ============================================================================

int find_input_intensity_quantile(const cv::Mat& frame, float percentage) {
    const int channels[] = {0};
    const int hist_size[] = {256};
    float range[] = {0, 256};
    const float* ranges[] = {range};
    cv::Mat hist;
    cv::calcHist(&frame, 1, channels, cv::Mat(), hist, 1, hist_size, ranges, true, false);
    
    int gray_level = 0;
    const int num_pixels_quantile = static_cast<int>(frame.total() * percentage * 0.01f);
    int num_pixels_accum = 0;
    for (int i = 0; i < hist_size[0]; ++i) {
        num_pixels_accum += static_cast<int>(hist.at<float>(i));
        if (num_pixels_accum > num_pixels_quantile) {
            gray_level = i;
            break;
        }
    }
    return gray_level;
}

void normalize_brightness(const cv::Mat& frame, cv::Mat& out) {
    const float pct = 90.0f;
    const int brightness = find_input_intensity_quantile(frame, pct);
    
    const double alpha = brightness < 127 
        ? (pct / 100.0f * 0.5f / std::max(5, brightness)) 
        : 1.0 / 255.0;
    const double beta = -0.5;
    
    frame.convertTo(out, CV_32F, alpha, beta);
}

static cv::Rect2f unnormalize_rect(const cv::Rect2f& r, int h, int w) {
    auto unnorm = [](float x) -> float { return 0.5f * (x + 1.0f); };
    auto tl = r.tl();
    auto br = r.br();
    float x0 = unnorm(tl.x) * w;
    float y0 = unnorm(tl.y) * h;
    float x1 = unnorm(br.x) * w;
    float y1 = unnorm(br.y) * h;
    return {x0, y0, x1 - x0, y1 - y0};
}

static cv::Size get_input_image_shape(const Ort::Session& session) {
    if (session.GetInputCount() < 1)
        throw std::invalid_argument("Model must take at least one input tensor");
    const std::vector<std::int64_t> shape = 
        session.GetInputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
    if (shape.size() != 4)
        throw std::invalid_argument("Model takes input tensor in wrong shape");
    return {static_cast<int>(shape[3]), static_cast<int>(shape[2])};
}

// ============================================================================
// Localizer implementation
// ============================================================================

Localizer::Localizer(Ort::MemoryInfo& allocator_info, Ort::Session&& session)
    : session_{std::move(session)}
    , scaled_frame_(INPUT_H, INPUT_W, CV_8U)
    , input_mat_(INPUT_H, INPUT_W, CV_32F)
{
    const std::int64_t input_shape[4] = {1, 1, INPUT_H, INPUT_W};
    input_val_ = Ort::Value::CreateTensor<float>(
        allocator_info, 
        input_mat_.ptr<float>(0), 
        input_mat_.total(), 
        input_shape, 
        4
    );
    
    const std::int64_t output_shape[2] = {1, 5};
    output_val_ = Ort::Value::CreateTensor<float>(
        allocator_info, 
        results_.data(), 
        results_.size(), 
        output_shape, 
        2
    );
}

std::pair<float, cv::Rect2f> Localizer::run(const cv::Mat& frame) {
    auto p = input_mat_.ptr(0);
    
    cv::resize(frame, scaled_frame_, {INPUT_W, INPUT_H}, 0, 0, cv::INTER_AREA);
    scaled_frame_.convertTo(input_mat_, CV_32F, 1.0 / 255.0, -0.5);
    
    assert(input_mat_.ptr(0) == p);
    
    const char* input_names[] = {"x"};
    const char* output_names[] = {"logit_box"};
    
    auto t_start = std::chrono::steady_clock::now();
    
    session_.Run(
        Ort::RunOptions{nullptr}, 
        input_names, &input_val_, 1, 
        output_names, &output_val_, 1
    );
    
    auto t_end = std::chrono::steady_clock::now();
    last_inference_time_ = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    
    const cv::Rect2f roi = unnormalize_rect(
        cv::Rect2f{
            results_[1], results_[2],
            results_[3] - results_[1],  // Width
            results_[4] - results_[2]   // Height
        },
        frame.rows, frame.cols
    );
    
    const float score = sigmoid(results_[0]);
    return {score, roi};
}

// ============================================================================
// PoseEstimator implementation
// ============================================================================

PoseEstimator::PoseEstimator(Ort::MemoryInfo& allocator_info, Ort::Session&& session)
    : model_version_{session.GetModelMetadata().GetVersion()}
    , session_{std::move(session)}
    , allocator_{session_, allocator_info}
{
    if (session_.GetOutputCount() < 2)
        throw std::runtime_error("Invalid Model: must have at least two outputs");
    
    // Handle uninitialized version field in old models
    if (model_version_ <= 0 || model_version_ > 4)
        model_version_ = 1;
    
    const cv::Size input_shape = get_input_image_shape(session_);
    scaled_frame_ = cv::Mat(input_shape, CV_8U, cv::Scalar(0));
    input_mat_ = cv::Mat(input_shape, CV_32F, cv::Scalar(0.f));
    
    // Create input tensor
    {
        const std::int64_t shape[4] = {1, 1, input_shape.height, input_shape.width};
        input_val_.push_back(Ort::Value::CreateTensor<float>(
            allocator_info, 
            input_mat_.ptr<float>(0), 
            input_mat_.total(), 
            shape, 
            4
        ));
    }
    
    // Collect input names first
    input_names_.reserve(session_.GetInputCount());
    for (size_t i = 0; i < session_.GetInputCount(); ++i) {
#if ORT_API_VERSION >= 12
        input_names_.push_back(std::string(&*session_.GetInputNameAllocated(i, allocator_)));
#else
        input_names_.push_back(std::string(session_.GetInputName(i, allocator_)));
#endif
    }
    
    // Collect output names and create tensors
    output_names_.reserve(session_.GetOutputCount());
    for (size_t i = 0; i < session_.GetOutputCount(); ++i) {
#if ORT_API_VERSION >= 12
        std::string name(&*session_.GetOutputNameAllocated(i, allocator_));
#else
        std::string name(session_.GetOutputName(i, allocator_));
#endif
        const auto& output_info = session_.GetOutputTypeInfo(i);
        const auto& onnx_spec = output_info.GetTensorTypeAndShapeInfo();
        const auto shape = onnx_spec.GetShape();
        
        // Create output tensor based on name
        if (name == "pos_size" && shape == std::vector<int64_t>{1, 3}) {
            output_val_.push_back(Ort::Value::CreateTensor<float>(
                allocator_info, &output_coord_[0], 3, shape.data(), shape.size()));
        } else if (name == "quat" && shape == std::vector<int64_t>{1, 4}) {
            output_val_.push_back(Ort::Value::CreateTensor<float>(
                allocator_info, &output_quat_[0], 4, shape.data(), shape.size()));
        } else if (name == "box" && shape == std::vector<int64_t>{1, 4}) {
            output_val_.push_back(Ort::Value::CreateTensor<float>(
                allocator_info, &output_box_[0], 4, shape.data(), shape.size()));
        } else {
            // Create a dummy tensor for unknown outputs
            auto t = Ort::Value::CreateTensor<float>(
                allocator_, shape.data(), shape.size());
            memset(t.GetTensorMutableData<float>(), 0, 
                   sizeof(float) * onnx_spec.GetElementCount());
            output_val_.push_back(std::move(t));
        }
        
        output_names_.push_back(name);
    }
    
    // NOW build c_str pointers AFTER vectors are complete (avoids dangling pointers)
    input_c_names_.reserve(input_names_.size());
    for (const auto& name : input_names_) {
        input_c_names_.push_back(name.c_str());
    }
    
    output_c_names_.reserve(output_names_.size());
    for (const auto& name : output_names_) {
        output_c_names_.push_back(name.c_str());
    }
}

std::optional<PoseEstimator::Face> PoseEstimator::run(
    const cv::Mat& frame, const cv::Rect& box) 
{
    const int patch_size = static_cast<int>(std::max(box.width, box.height) * 1.05f);
    const cv::Point2f patch_center = {
        std::clamp<float>(box.x + 0.5f * box.width, 0.f, static_cast<float>(frame.cols)),
        std::clamp<float>(box.y + 0.5f * box.height, 0.f, static_cast<float>(frame.rows))
    };
    
    cv::Mat cropped;
    cv::getRectSubPix(frame, {patch_size, patch_size}, patch_center, cropped);
    
    if (cropped.rows != patch_size || cropped.cols != patch_size)
        return {};
    
    cv::resize(cropped, scaled_frame_, scaled_frame_.size(), 0, 0, cv::INTER_AREA);
    normalize_brightness(scaled_frame_, input_mat_);
    
    auto t_start = std::chrono::steady_clock::now();
    
    try {
        session_.Run(
            Ort::RunOptions{nullptr},
            input_c_names_.data(), input_val_.data(), input_val_.size(),
            output_c_names_.data(), output_val_.data(), output_val_.size()
        );
    } catch (const Ort::Exception& e) {
        ltr_int_log_message("Pose model error: %s\n", e.what());
        return {};
    }
    
    auto t_end = std::chrono::steady_clock::now();
    last_inference_time_ = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    
    // Convert coordinates from patch-local to frame coordinates
    const cv::Point2f center = patch_center + 
        (0.5f * patch_size) * cv::Point2f{output_coord_[0], output_coord_[1]};
    const float size = patch_size * 0.5f * output_coord_[2];
    
    // Quaternion: OpenCV/Eigen order is (w, x, y, z)
    cv::Quatf rotation = {output_quat_[3], output_quat_[0], output_quat_[1], output_quat_[2]};
    
    const cv::Rect2f outbox = {
        patch_center.x + (0.5f * patch_size) * output_box_[0],
        patch_center.y + (0.5f * patch_size) * output_box_[1],
        0.5f * patch_size * (output_box_[2] - output_box_[0]),
        0.5f * patch_size * (output_box_[3] - output_box_[1])
    };
    
    return Face{rotation, outbox, center, size};
}

// ============================================================================
// NeuralNetTracker implementation
// ============================================================================

NeuralNetTracker::NeuralNetTracker() = default;
NeuralNetTracker::~NeuralNetTracker() = default;

bool NeuralNetTracker::init(const std::string& localizer_path, const std::string& posenet_path) {
    try {
        env_ = Ort::Env{OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, "ltr_neuralnet"};
        alloc_info_ = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
        
        Ort::SessionOptions opts;
        opts.SetIntraOpNumThreads(1);
        opts.SetInterOpNumThreads(1);
        
        ltr_int_log_message("Loading localizer model: %s\n", localizer_path.c_str());
        localizer_ = std::make_unique<Localizer>(
            alloc_info_, 
            Ort::Session{env_, localizer_path.c_str(), opts}
        );
        
        ltr_int_log_message("Loading posenet model: %s\n", posenet_path.c_str());
        posenet_ = std::make_unique<PoseEstimator>(
            alloc_info_,
            Ort::Session{env_, posenet_path.c_str(), opts}
        );
        
        ltr_int_log_message("Neural net tracker initialized successfully\n");
        return true;
        
    } catch (const std::exception& e) {
        ltr_int_log_message("Failed to initialize neural net tracker: %s\n", e.what());
        localizer_.reset();
        posenet_.reset();
        return false;
    }
}

void NeuralNetTracker::set_fov(float diagonal_fov_degrees) {
    // Assume 4:3 aspect ratio for typical webcam
    const float diag_fov = diagonal_fov_degrees * static_cast<float>(M_PI) / 180.0f;
    const float fov_w = 2.0f * std::atan(std::tan(diag_fov / 2.0f) / std::sqrt(1.0f + 0.75f * 0.75f));
    const float fov_h = 2.0f * std::atan(std::tan(diag_fov / 2.0f) / std::sqrt(1.0f + 4.0f/3.0f * 4.0f/3.0f));
    focal_length_w_ = 1.0f / std::tan(0.5f * fov_w);
    focal_length_h_ = 1.0f / std::tan(0.5f * fov_h);
}

float NeuralNetTracker::iou(const cv::Rect2f& a, const cv::Rect2f& b) {
    auto intersection = a & b;
    float inter_area = intersection.area();
    float union_area = a.area() + b.area() - inter_area;
    return union_area > 0 ? inter_area / union_area : 0.0f;
}

cv::Quatf NeuralNetTracker::image_to_world_quat(const cv::Quatf& q) {
    // Transform from image coordinates to world coordinates
    // Swap and negate axes as needed
    return cv::Quatf{q.w, -q.z, -q.y, q.x};
}

void NeuralNetTracker::quaternion_to_euler(const cv::Quatf& q, 
                                            float& pitch, float& yaw, float& roll) {
    // Convert quaternion to rotation matrix
    cv::Matx33f R = q.toRotMat3x3(cv::QUAT_ASSUME_UNIT);
    
    // Extract Euler angles (YZX convention matching OpenTrack)
    // For the NN tracker x is forward, y is up
    const auto& mx = R.col(0);
    const auto& my = R.col(1);
    const auto& mz = R.col(2);
    
    yaw = std::atan2(mx(2), mx(0));
    pitch = -std::atan2(-mx(1), std::sqrt(mx(2)*mx(2) + mx(0)*mx(0)));
    roll = std::atan2(-mz(1), my(1));
    
    // Convert to degrees
    constexpr float rad2deg = 180.0f / static_cast<float>(M_PI);
    pitch *= rad2deg;
    yaw *= rad2deg;
    roll *= -rad2deg;  // Negate roll to match convention
}

void NeuralNetTracker::image_to_world_position(const cv::Point2f& center, float face_size,
                                                const cv::Size& image_size,
                                                float& tx, float& ty, float& tz) {
    // Compute 3D position from 2D location and face size
    // Uses pinhole camera model
    const float head_size_vertical = 2.0f * face_size;
    
    // X position (depth) from head size
    const float xpos = -(focal_length_w_ * image_size.width * 0.5f) / 
                       head_size_vertical * HEAD_SIZE_MM;
    
    // Z and Y position from screen coordinates
    const float zpos = (center.x / image_size.width * 2.0f - 1.0f) * xpos / focal_length_w_;
    const float ypos = (center.y / image_size.height * 2.0f - 1.0f) * xpos / focal_length_h_;
    
    // Map to linuxtrack conventions (convert mm to cm for output)
    tx = -zpos * 0.1f;   // Right/left
    ty = ypos * 0.1f;    // Up/down
    tz = -xpos * 0.1f;   // Forward/back (depth)
}

bool NeuralNetTracker::detect(const cv::Mat& frame,
                               float& pitch, float& yaw, float& roll,
                               float& tx, float& ty, float& tz) {
    if (!localizer_ || !posenet_) {
        last_valid_ = false;
        return false;
    }
    
    // Convert to grayscale if needed
    cv::Mat gray;
    if (frame.channels() == 3 || frame.channels() == 4) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = frame;
    }
    
    // Run localizer if needed (ROI doesn't match or first frame)
    if (!last_localizer_roi_ || !last_roi_ || iou(*last_localizer_roi_, *last_roi_) < 0.25f) {
        auto [score, roi] = localizer_->run(gray);
        
        if (last_roi_ && iou(roi, *last_roi_) >= 0.25f && score > 0.5f) {
            // ROI matches tracking, just update localizer result
            last_localizer_roi_ = roi;
        } else if (score > 0.5f && roi.height > 32 && roi.width > 32) {
            // New face detected, reset tracking
            last_localizer_roi_ = roi;
            last_roi_ = roi;
        } else {
            // No face found
            last_roi_.reset();
            last_localizer_roi_.reset();
            last_valid_ = false;
            return false;
        }
    }
    
    if (!last_roi_) {
        last_valid_ = false;
        return false;
    }
    
    // Run pose estimator on the ROI
    auto face = posenet_->run(gray, cv::Rect(*last_roi_));
    
    if (!face) {
        last_roi_.reset();
        last_valid_ = false;
        return false;
    }
    
    // Update ROI for next frame (with smoothing)
    const float roi_alpha = 0.5f;
    cv::Rect2f expanded_box = face->box;
    expanded_box.x -= expanded_box.width * 0.1f;
    expanded_box.y -= expanded_box.height * 0.1f;
    expanded_box.width *= 1.2f;
    expanded_box.height *= 1.2f;
    
    last_roi_ = cv::Rect2f{
        last_roi_->x + roi_alpha * (expanded_box.x - last_roi_->x),
        last_roi_->y + roi_alpha * (expanded_box.y - last_roi_->y),
        last_roi_->width + roi_alpha * (expanded_box.width - last_roi_->width),
        last_roi_->height + roi_alpha * (expanded_box.height - last_roi_->height)
    };
    
    // Convert quaternion to world space and then to Euler angles
    cv::Quatf world_quat = image_to_world_quat(face->rotation);
    quaternion_to_euler(world_quat, pitch, yaw, roll);
    
    // Convert image position to world translation
    image_to_world_position(face->center, face->size, gray.size(), tx, ty, tz);
    
    // Store for visualization
    last_pitch_ = pitch;
    last_yaw_ = yaw;
    last_roll_ = roll;
    last_tx_ = tx;
    last_ty_ = ty;
    last_tz_ = tz;
    last_valid_ = true;
    
    return true;
}

double NeuralNetTracker::last_localizer_time_ms() const {
    return localizer_ ? localizer_->last_inference_time_ms() : 0.0;
}

double NeuralNetTracker::last_posenet_time_ms() const {
    return posenet_ ? posenet_->last_inference_time_ms() : 0.0;
}

void NeuralNetTracker::draw_gizmos(cv::Mat& frame) const {
    if (!last_valid_ || !last_roi_)
        return;
    
    // Draw face bounding box
    cv::rectangle(frame, *last_roi_, cv::Scalar(0, 255, 0), 2);
    
    // Draw pose info text
    char text[128];
    snprintf(text, sizeof(text), "P:%.1f Y:%.1f R:%.1f", 
             last_pitch_, last_yaw_, last_roll_);
    cv::putText(frame, text, cv::Point(10, 30), 
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    
    snprintf(text, sizeof(text), "Tx:%.1f Ty:%.1f Tz:%.1f", 
             last_tx_, last_ty_, last_tz_);
    cv::putText(frame, text, cv::Point(10, 60), 
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
}

} // namespace ltr_neuralnet

#endif // HAVE_ONNXRUNTIME
