#include "catch2/catch_amalgamated.hpp"

extern "C" {
#include "../image_process.h"
#include "../capture_replay.h"
}

namespace {

int g_process_calls = 0;
bool g_last_face_tracking = false;
bool g_last_flip = false;
int g_last_min_blob = 0;
int g_last_max_blob = 0;
bloblist_type *g_last_bloblist = nullptr;
image_t g_last_image{};

void reset_capture_replay_stubs() {
  g_process_calls = 0;
  g_last_face_tracking = false;
  g_last_flip = false;
  g_last_min_blob = 0;
  g_last_max_blob = 0;
  g_last_bloblist = nullptr;
  g_last_image = {};
}

} // namespace

extern "C" {

int ltr_int_process_gray_image(image_t *img, struct bloblist_type *bloblist,
                               int min_blob_pixels, int max_blob_pixels,
                               bool flip, bool face_tracking) {
  ++g_process_calls;
  g_last_image = *img;
  g_last_bloblist = bloblist;
  g_last_min_blob = min_blob_pixels;
  g_last_max_blob = max_blob_pixels;
  g_last_flip = flip;
  g_last_face_tracking = face_tracking;
  bloblist->num_blobs = 1;
  return 0;
}

} // extern "C"

TEST_CASE("gray capture replay thresholds non-face frames before processing",
          "[capture_replay]") {
  reset_capture_replay_stubs();
  const unsigned char gray_bitmap[] = {10, 42, 43, 255};
  unsigned char preview_fallback[4] = {};
  unsigned char tracking_fallback[4] = {};
  blob_type blobs[3] = {};
  frame_type frame{};
  frame.bloblist.blobs = blobs;
  frame.bloblist.expected_blobs = 3;

  ltr_gray_capture_frame input{
      .gray_bitmap = gray_bitmap,
      .width = 2,
      .height = 2,
      .expected_blobs = 3,
      .threshold = 42,
      .min_blob_pixels = 10,
      .max_blob_pixels = 20,
      .flip = true,
      .face_tracking = false,
  };

  REQUIRE(ltr_int_replay_gray_capture_frame(&input, &frame, preview_fallback,
                                            tracking_fallback) == 0);

  CHECK(frame.width == 2);
  CHECK(frame.height == 2);
  CHECK(frame.camera_diag[0] == '\0');
  CHECK(frame.camera_diag2[0] == '\0');
  CHECK(preview_fallback[0] == 10);
  CHECK(preview_fallback[1] == 42);
  CHECK(preview_fallback[2] == 43);
  CHECK(preview_fallback[3] == 255);
  CHECK(tracking_fallback[0] == 0);
  CHECK(tracking_fallback[1] == 0);
  CHECK(tracking_fallback[2] == 43);
  CHECK(tracking_fallback[3] == 255);
  CHECK(g_process_calls == 1);
  CHECK(g_last_image.bitmap == tracking_fallback);
  CHECK(g_last_bloblist == &frame.bloblist);
  CHECK(g_last_min_blob == 10);
  CHECK(g_last_max_blob == 20);
  CHECK(g_last_flip);
  CHECK_FALSE(g_last_face_tracking);
}

TEST_CASE("gray capture replay preserves face frames for face processing",
          "[capture_replay]") {
  reset_capture_replay_stubs();
  const unsigned char gray_bitmap[] = {10, 42, 43, 255};
  unsigned char preview_owned[4] = {};
  unsigned char tracking_owned[4] = {};
  blob_type blobs[3] = {};
  frame_type frame{};
  frame.bitmap = preview_owned;
  frame.bitmap_processed = tracking_owned;
  frame.bloblist.blobs = blobs;
  frame.bloblist.expected_blobs = 3;

  ltr_gray_capture_frame input{
      .gray_bitmap = gray_bitmap,
      .width = 2,
      .height = 2,
      .expected_blobs = 1,
      .threshold = 99,
      .min_blob_pixels = 0,
      .max_blob_pixels = 0,
      .flip = false,
      .face_tracking = true,
  };

  REQUIRE(ltr_int_replay_gray_capture_frame(&input, &frame, nullptr,
                                            nullptr) == 0);

  CHECK(frame.bloblist.num_blobs == 1);
  CHECK(preview_owned[0] == 10);
  CHECK(preview_owned[1] == 42);
  CHECK(preview_owned[2] == 43);
  CHECK(preview_owned[3] == 255);
  CHECK(tracking_owned[0] == 10);
  CHECK(tracking_owned[1] == 42);
  CHECK(tracking_owned[2] == 43);
  CHECK(tracking_owned[3] == 255);
  CHECK(g_process_calls == 1);
  CHECK(g_last_image.bitmap == tracking_owned);
  CHECK_FALSE(g_last_flip);
  CHECK(g_last_face_tracking);
}
