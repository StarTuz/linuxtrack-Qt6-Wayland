#include "webcam_driver.h"
#include "wc_driver_prefs.h"
#include "utils.h"
#include "image_process.h"
#ifdef OPENCV
#include "facetrack.h"
#endif
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>
#include <string>

// Global capture object
static cv::VideoCapture cap;
static cv::Mat frame;
static cv::Mat gray;
static int current_width = 0;
static int current_height = 0;

// Get camera name from V4L2 device
static std::string get_camera_name(const char* device_path) {
    int fd = open(device_path, O_RDONLY);
    if (fd < 0) {
        return "";
    }
    
    struct v4l2_capability capability;
    if (ioctl(fd, VIDIOC_QUERYCAP, &capability) == 0) {
        // Check if it supports video capture and streaming
        if ((capability.capabilities & V4L2_CAP_VIDEO_CAPTURE) &&
            (capability.capabilities & V4L2_CAP_STREAMING)) {
            close(fd);
            std::string name = reinterpret_cast<const char*>(capability.card);
            // Strip leading whitespace
            size_t start = name.find_first_not_of(" \t");
            if (start != std::string::npos) {
                name = name.substr(start);
            }
            return name;
        }
    }
    close(fd);
    return "";
}

// Internal helper to enumerate cameras
static std::vector<std::string> enumerate_cameras() {
    std::vector<std::string> cameras;
    char device[32];
    for(int i=0; i<10; ++i) {
        snprintf(device, sizeof(device), "/dev/video%d", i);
        if(access(device, F_OK) == 0) {
            std::string name = get_camera_name(device);
            if (!name.empty()) {
                // Deduplicate - only add if not already in list
                bool found = false;
                for (const auto& cam : cameras) {
                    if (cam == name) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cameras.push_back(name);
                }
            }
        }
    }
    return cameras;
}

extern "C" {

int ltr_int_enum_webcams(char **ids[]) {
    std::vector<std::string> cams = enumerate_cameras();
    *ids = (char**)ltr_int_my_malloc((cams.size() + 1) * sizeof(char*));
    for(size_t i=0; i<cams.size(); ++i) {
        (*ids)[i] = ltr_int_my_strdup(cams[i].c_str());
    }
    (*ids)[cams.size()] = NULL;
    return cams.size();
}

int ltr_int_enum_webcam_formats(const char *id, webcam_formats *all_formats) {
    // OpenCV doesn't really expose format enumeration easily before opening.
    // For prototype, we report generic supported resolutions.
    // This is a known limitation of using high-level OpenCV API.
    
    webcam_format generic_formats[] = {
        {0, 0 /*MJPG*/, 640, 480, 30, 1},
        {1, 0 /*MJPG*/, 320, 240, 30, 1},
        {2, 0 /*MJPG*/, 640, 480, 60, 1}, // Example
        {3, 0 /*MJPG*/, 1280, 720, 30, 1},
        {4, 0 /*MJPG*/, 1920, 1080, 30, 1}
    };
    
    int num_gen = 5;
    
    all_formats->entries = num_gen;
    all_formats->formats = (webcam_format*)ltr_int_my_malloc(num_gen * sizeof(webcam_format));
    all_formats->fmt_strings = (char**)ltr_int_my_malloc((num_gen + 1) * sizeof(char*));
    
    for(int i=0; i<num_gen; ++i) {
        all_formats->formats[i] = generic_formats[i];
        asprintf(&(all_formats->fmt_strings[i]), "%dx%d @ %d fps (OpenCV Auto)", 
                 generic_formats[i].w, generic_formats[i].h, generic_formats[i].fps_num);
    }
    all_formats->fmt_strings[num_gen] = NULL;
    return num_gen;
}

int ltr_int_enum_webcam_formats_cleanup(webcam_formats *all_formats) {
    for(int i=0; i<all_formats->entries; ++i) {
        free(all_formats->fmt_strings[i]);
    }
    free(all_formats->fmt_strings);
    free(all_formats->formats);
    return 0;
}

int ltr_int_tracker_init(struct camera_control_block *ccb) {
    if(cap.isOpened()) {
        cap.release();
    }
    
    // ccb->device.device_id contains camera name (e.g., "Logitech C920")
    // We need to find the matching /dev/videoN device
    
    int device_index = -1;
    std::string target_name = ccb->device.device_id;
    
    // If it's already a /dev/videoN path, extract the index directly
    if(target_name.find("/dev/video") == 0) {
        device_index = std::stoi(target_name.substr(10));
    } else {
        // Search for matching camera name
        char device[32];
        for(int i=0; i<10; ++i) {
            snprintf(device, sizeof(device), "/dev/video%d", i);
            if(access(device, F_OK) == 0) {
                std::string name = get_camera_name(device);
                if (!name.empty() && name == target_name) {
                    device_index = i;
                    break;
                }
            }
        }
    }
    
    if (device_index < 0) {
        ltr_int_log_message("Could not find camera '%s'\n", ccb->device.device_id);
        return -1;
    }
    
    // Try to open with ANY backend
    if(!cap.open(device_index, cv::CAP_ANY)) {
        ltr_int_log_message("OpenCV failed to open device index %d\n", device_index);
        return -1;
    }
    
    // Set Preferences
    // In real implementation, map prefs to cap.set()
    // For now, try to set from CCB pixel width/height which usually comes from prefs
    int w = 640, h = 480;
    ltr_int_wc_get_resolution(&w, &h);
    
    cap.set(cv::CAP_PROP_FRAME_WIDTH, w);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, h);
    
    // Always request MJPG to be efficient
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    
    // Check what we got
    current_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    current_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    
    ccb->pixel_width = current_width;
    ccb->pixel_height = current_height;
    
    ltr_int_prepare_for_processing(current_width, current_height);
    
    ltr_int_log_message("OpenCV Webcam initialized: %dx%d\n", current_width, current_height);
    
#ifdef OPENCV
    if(!ltr_int_init_face_detect()) {
        ltr_int_log_message("Face detection initialization failed!\n");
    }
#endif
    return 0;
}

int ltr_int_tracker_shutdown() { // Header signature
    if(cap.isOpened()) {
        cap.release();
    }
#ifdef OPENCV
    ltr_int_stop_face_detect();
#endif
    ltr_int_cleanup_after_processing();
    return 0;
}

// Forward to shutdown for close call from runloop
int ltr_int_tracker_close() {
    return ltr_int_tracker_shutdown();
}

int ltr_int_tracker_suspend() {
    // OpenCV generally doesn't have a "suspend" that keeps device open but stops stream
    // Release might be safest, or just ignore frames.
    // Linuxtrack expects stream off.
    // cap.set(cv::CAP_PROP_SETTINGS, ...); // No standard pause
    return 0;
}

int ltr_int_tracker_pause() {
    // Similar to suspend
    return 0;
}

int ltr_int_tracker_resume() {
    if(!cap.isOpened()) {
        // Re-open if closed? For now assume it stays open.
    }
    return 0;
}

int ltr_int_tracker_wakeup() {
    return ltr_int_tracker_resume();
}

int ltr_int_tracker_get_frame(struct camera_control_block *ccb, struct frame_type *f, bool *frame_acquired) {
    if(!cap.isOpened()) {
        *frame_acquired = false;
        return -1;
    }
    
    if(cap.read(frame)) {
        *frame_acquired = true;

        // debug fps
        static int64_t last_time = 0;
        static int frames = 0;
        int64_t now = cv::getTickCount();
        frames++;
        if((now - last_time)/cv::getTickFrequency() > 1.0) {
            ltr_int_log_message("Webcam FPS: %d\n", frames);
            frames = 0;
            last_time = now;
        }
        
        // Convert to Grayscale for Face Tracker processing
        if(frame.channels() == 3) {
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        } else {
            gray = frame.clone();
        }
        
        // Ensure size matches
        if(gray.cols != current_width || gray.rows != current_height) {
            cv::resize(gray, gray, cv::Size(current_width, current_height));
        }
        
        // Copy to buffer
        // f->bitmap is typically managed by ltr_int_frame_free/malloc logic in runloop? 
        // No, runloop provides initialized frame struct, usually with bitmap=NULL, then we provide buffer?
        // Wait, runloop::ltr_int_rl_run -> ltr_int_tracker_get_frame
        // webcam_driver.c normally copies to f->bitmap if not NULL, or internal buffer
        
        // We need to return a buffer accessible to the caller.
        // Let's use internal static buffer for prototype simplicity, similar to wc_info.bw_frame
        
        static std::vector<uint8_t> internal_buffer;
        if(internal_buffer.size() != (size_t)(current_width * current_height)) {
            internal_buffer.resize(current_width * current_height);
        }
        
        memcpy(internal_buffer.data(), gray.data, internal_buffer.size());
        
        f->bitmap = internal_buffer.data();
        f->width = current_width;
        f->height = current_height;
        
#ifdef OPENCV
        // Create image_t for face detection (only for libft, not libwc)
        image_t img = {
            .w = current_width,
            .h = current_height,
            .bitmap = internal_buffer.data(),
            .ratio = 1.0f
        };
        
        static int face_log_cnt = 0;
        if(++face_log_cnt % 60 == 0) {
            ltr_int_log_message("Calling ltr_int_face_detect: %dx%d bitmap=%p\n", 
                                current_width, current_height, img.bitmap);
            ltr_int_log_message("Pixel[0,0]=%d [W/2,H/2]=%d\n", 
                                internal_buffer[0], 
                                internal_buffer[(current_height/2) * current_width + (current_width/2)]);
        }
        
        // Call face detection - this also draws the rectangle on the display
        ltr_int_face_detect(&img, &(f->bloblist));
#else
        // For libwc (blob tracking), use traditional stripe-based detection
        f->bloblist.num_blobs = 0;
#endif
        
        return 0;
    } else {
        *frame_acquired = false;
        return 0;
    }
}

} // extern "C"
