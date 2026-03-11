#include "catch2/catch_amalgamated.hpp"

#include <vector>

extern "C" {
#include "../cal.h"
#include "../image_process.h"
}

TEST_CASE("webcam blob limits scale from the 160x120 reference frame") {
  image_t img{.w = 800, .h = 600, .bitmap = nullptr, .ratio = 1.0f};
  int min_blob = 4;
  int max_blob = 230;

  ltr_int_scale_blob_limits_for_resolution(&img, &min_blob, &max_blob);

  REQUIRE(min_blob == 100);
  REQUIRE(max_blob == 5750);
}

TEST_CASE("shared blob extraction keeps caller thresholds unchanged for 640x480") {
  constexpr int kWidth = 640;
  constexpr int kHeight = 480;
  std::vector<unsigned char> bitmap(kWidth * kHeight, 0);

  // Create one 20x15 bright block => 300 pixels.
  for (int y = 100; y < 115; ++y) {
    for (int x = 200; x < 220; ++x) {
      bitmap[(y * kWidth) + x] = 0xFF;
    }
  }

  image_t img{
      .w = kWidth,
      .h = kHeight,
      .bitmap = bitmap.data(),
      .ratio = 1.0f,
  };

  blob_type blobs[3] = {};
  bloblist_type bloblist{
      .num_blobs = 0,
      .expected_blobs = 3,
      .blobs = blobs,
  };

  ltr_int_prepare_for_processing(kWidth, kHeight);
  ltr_int_to_stripes(&img);
  REQUIRE(ltr_int_stripes_to_blobs(3, &bloblist, 120, 2500, &img) == 0);
  ltr_int_cleanup_after_processing();

  REQUIRE(bloblist.num_blobs == 1);
  REQUIRE(bloblist.blobs[0].score == 300);
}
