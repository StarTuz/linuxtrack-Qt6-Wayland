#include "catch2/catch_amalgamated.hpp"

extern "C" {
#include "../capture_provider.h"
}

namespace {

int g_next_frame_calls = 0;
int g_replay_calls = 0;
int g_next_frame_result = 1;
int g_replay_result = 0;
ltr_gray_capture_frame g_stub_frame{};
ltr_gray_capture_frame g_last_replayed_frame{};
frame_type *g_last_frame_ptr = nullptr;
unsigned char *g_last_preview_fallback = nullptr;
unsigned char *g_last_tracking_fallback = nullptr;

void reset_capture_provider_stubs() {
  g_next_frame_calls = 0;
  g_replay_calls = 0;
  g_next_frame_result = 1;
  g_replay_result = 0;
  g_stub_frame = {};
  g_last_replayed_frame = {};
  g_last_frame_ptr = nullptr;
  g_last_preview_fallback = nullptr;
  g_last_tracking_fallback = nullptr;
}

int stub_next_frame(void * /*ctx*/, ltr_gray_capture_frame *frame) {
  ++g_next_frame_calls;
  *frame = g_stub_frame;
  return g_next_frame_result;
}

const ltr_gray_capture_provider_vtable k_vtable = {
    .next_frame = stub_next_frame,
};

} // namespace

extern "C" {

int ltr_int_replay_gray_capture_frame(const ltr_gray_capture_frame *input,
                                      struct frame_type *frame,
                                      unsigned char *preview_fallback,
                                      unsigned char *tracking_fallback) {
  ++g_replay_calls;
  g_last_replayed_frame = *input;
  g_last_frame_ptr = frame;
  g_last_preview_fallback = preview_fallback;
  g_last_tracking_fallback = tracking_fallback;
  return g_replay_result;
}

} // extern "C"

TEST_CASE("capture provider forwards acquired frames into replay path",
          "[capture_provider]") {
  reset_capture_provider_stubs();
  static const unsigned char bitmap[] = {1, 2, 3, 4};
  frame_type frame{};
  bool frame_acquired = false;
  unsigned char preview_fallback[4] = {};
  unsigned char tracking_fallback[4] = {};
  ltr_gray_capture_provider provider{
      .vtable = &k_vtable,
      .ctx = nullptr,
  };

  g_stub_frame.gray_bitmap = bitmap;
  g_stub_frame.width = 2;
  g_stub_frame.height = 2;
  g_stub_frame.expected_blobs = 3;
  g_stub_frame.threshold = 42;
  g_stub_frame.min_blob_pixels = 10;
  g_stub_frame.max_blob_pixels = 20;
  g_stub_frame.flip = true;
  g_stub_frame.face_tracking = false;

  REQUIRE(ltr_int_provider_get_frame(&provider, &frame, preview_fallback,
                                     tracking_fallback, &frame_acquired) == 0);

  CHECK(frame_acquired);
  CHECK(g_next_frame_calls == 1);
  CHECK(g_replay_calls == 1);
  CHECK(g_last_replayed_frame.gray_bitmap == bitmap);
  CHECK(g_last_replayed_frame.width == 2);
  CHECK(g_last_frame_ptr == &frame);
  CHECK(g_last_preview_fallback == preview_fallback);
  CHECK(g_last_tracking_fallback == tracking_fallback);
}

TEST_CASE("capture provider preserves no-frame and error results",
          "[capture_provider]") {
  reset_capture_provider_stubs();
  frame_type frame{};
  bool frame_acquired = true;
  ltr_gray_capture_provider provider{
      .vtable = &k_vtable,
      .ctx = nullptr,
  };

  g_next_frame_result = 0;
  CHECK(ltr_int_provider_get_frame(&provider, &frame, nullptr, nullptr,
                                   &frame_acquired) == 0);
  CHECK_FALSE(frame_acquired);
  CHECK(g_replay_calls == 0);

  frame_acquired = true;
  g_next_frame_result = -1;
  CHECK(ltr_int_provider_get_frame(&provider, &frame, nullptr, nullptr,
                                   &frame_acquired) == -1);
  CHECK_FALSE(frame_acquired);
  CHECK(g_replay_calls == 0);
}

TEST_CASE("capture provider reports replay failures without marking a frame",
          "[capture_provider]") {
  reset_capture_provider_stubs();
  frame_type frame{};
  bool frame_acquired = false;
  ltr_gray_capture_provider provider{
      .vtable = &k_vtable,
      .ctx = nullptr,
  };

  g_replay_result = -1;
  CHECK(ltr_int_provider_get_frame(&provider, &frame, nullptr, nullptr,
                                   &frame_acquired) == -1);
  CHECK_FALSE(frame_acquired);
  CHECK(g_next_frame_calls == 1);
  CHECK(g_replay_calls == 1);
}
