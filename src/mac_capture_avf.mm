#include "mac_capture_avf.h"
#include "utils.h"
#include <mutex>
#include <condition_variable>

#import <AVFoundation/AVFoundation.h>
#import <CoreVideo/CoreVideo.h>
#import <Foundation/Foundation.h>

@interface LtrAVFCaptureDelegate : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate> {
    AVCaptureSession *session;
    std::mutex frame_mutex;
    ltr_gray_capture_frame pending_frame;
    bool frame_ready;
    uint8_t *gray_buffer;
    size_t buffer_size;
}

- (BOOL)startCapture;
- (void)stopCapture;
- (int)getNextFrame:(ltr_gray_capture_frame *)frame;

@end

@implementation LtrAVFCaptureDelegate

- (instancetype)init {
    self = [super init];
    if (self) {
        frame_ready = false;
        gray_buffer = NULL;
        buffer_size = 0;
    }
    return self;
}

- (void)dealloc {
    [self stopCapture];
    if (gray_buffer) {
        free(gray_buffer);
    }
    [super dealloc];
}

- (BOOL)startCapture {
    session = [[AVCaptureSession alloc] init];
    if (!session) return NO;
    
    [session setSessionPreset:AVCaptureSessionPreset640x480];
    
    // Find default video device
    AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    if (!device) {
        ltr_int_log_message("macOS AVFoundation: No video device found\n");
        return NO;
    }
    
    NSError *error = nil;
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
    if (!input || error) {
        ltr_int_log_message("macOS AVFoundation: Could not open video device\n");
        return NO;
    }
    
    if ([session canAddInput:input]) {
        [session addInput:input];
    } else {
        return NO;
    }
    
    AVCaptureVideoDataOutput *output = [[AVCaptureVideoDataOutput alloc] init];
    // Output standard BGRA or YUV. Let's ask for BGRA and we convert to gray.
    output.videoSettings = @{ (id)kCVPixelBufferPixelFormatTypeKey : @(kCVPixelFormatType_32BGRA) };
    output.alwaysDiscardsLateVideoFrames = YES;
    
    dispatch_queue_t queue = dispatch_queue_create("LtrAVFCaptureQueue", NULL);
    [output setSampleBufferDelegate:self queue:queue];
    
    if ([session canAddOutput:output]) {
        [session addOutput:output];
    } else {
        return NO;
    }
    
    [session startRunning];
    return YES;
}

- (void)stopCapture {
    if (session) {
        [session stopRunning];
        session = nil;
    }
}

- (void)captureOutput:(AVCaptureOutput *)output didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    if (!imageBuffer) return;
    
    CVPixelBufferLockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    
    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t height = CVPixelBufferGetHeight(imageBuffer);
    size_t src_bytes_per_row = CVPixelBufferGetBytesPerRow(imageBuffer);
    uint8_t *src_base = (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer);
    
    size_t needed_size = width * height;
    
    std::lock_guard<std::mutex> lock(frame_mutex);
    
    if (buffer_size < needed_size || !gray_buffer) {
        if (gray_buffer) free(gray_buffer);
        gray_buffer = (uint8_t *)malloc(needed_size);
        buffer_size = needed_size;
    }
    
    // Convert BGRA to grayscale
    // Y' = 0.299 R + 0.587 G + 0.114 B (fast approximation)
    for (size_t y = 0; y < height; ++y) {
        uint8_t *src_row = src_base + y * src_bytes_per_row;
        uint8_t *dst_row = gray_buffer + y * width;
        for (size_t x = 0; x < width; ++x) {
            uint8_t b = src_row[x * 4 + 0];
            uint8_t g = src_row[x * 4 + 1];
            uint8_t r = src_row[x * 4 + 2];
            dst_row[x] = (uint8_t)((r * 299 + g * 587 + b * 114) / 1000);
        }
    }
    
    pending_frame.gray_bitmap = gray_buffer;
    pending_frame.width = (unsigned int)width;
    pending_frame.height = (unsigned int)height;
    pending_frame.expected_blobs = 3;  // Default for trackir
    pending_frame.threshold = 128;     // Arbitrary initial threshold
    pending_frame.min_blob_pixels = 4;
    pending_frame.max_blob_pixels = 400;
    pending_frame.flip = false;
    pending_frame.face_tracking = false;
    
    frame_ready = true;
    
    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
}

- (int)getNextFrame:(ltr_gray_capture_frame *)frame {
    std::lock_guard<std::mutex> lock(frame_mutex);
    if (!frame_ready) return 0;
    if (frame) {
        *frame = pending_frame;
    }
    frame_ready = false;
    return 1;
}

@end

static int mac_capture_avf_next_frame(void *ctx, ltr_gray_capture_frame *frame) {
    ltr_mac_capture_avf *avf = (ltr_mac_capture_avf *)ctx;
    if (!avf || !avf->internal) return -1;
    
    LtrAVFCaptureDelegate *delegate = (LtrAVFCaptureDelegate *)avf->internal;
    return [delegate getNextFrame:frame];
}

static const ltr_gray_capture_provider_vtable k_mac_capture_avf_vtable = {
    .next_frame = mac_capture_avf_next_frame,
};

bool ltr_int_mac_capture_avf_init(ltr_mac_capture_avf *avf) {
    if (!avf) return false;
    
    avf->provider.vtable = &k_mac_capture_avf_vtable;
    avf->provider.ctx = avf;
    
    LtrAVFCaptureDelegate *delegate = [[LtrAVFCaptureDelegate alloc] init];
    if (![delegate startCapture]) {
        [delegate release];
        return false;
    }
    
    // Retain it for C struct
    avf->internal = (void *)delegate;
    return true;
}

void ltr_int_mac_capture_avf_shutdown(ltr_mac_capture_avf *avf) {
    if (!avf || !avf->internal) return;
    
    LtrAVFCaptureDelegate *delegate = (LtrAVFCaptureDelegate *)avf->internal;
    [delegate stopCapture];
    [delegate release];
    avf->internal = NULL;
}

const ltr_gray_capture_provider *ltr_int_mac_capture_avf_provider(ltr_mac_capture_avf *avf) {
    if (!avf) return NULL;
    return &(avf->provider);
}
