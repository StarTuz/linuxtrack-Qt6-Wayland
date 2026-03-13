#include "catch2/catch_amalgamated.hpp"

extern "C" {
#include "../mac_capture_stub.h"
}

TEST_CASE("mac capture stub exposes a provider after initialization",
          "[mac_capture_stub]") {
  ltr_mac_capture_stub stub{};

  ltr_int_mac_capture_stub_init(&stub);

  REQUIRE(ltr_int_mac_capture_stub_provider(&stub) != nullptr);
  CHECK_FALSE(stub.frame_ready);
}

TEST_CASE("mac capture stub yields one submitted frame and then empties",
          "[mac_capture_stub]") {
  static const unsigned char bitmap[] = {1, 2, 3, 4};
  ltr_mac_capture_stub stub{};
  ltr_gray_capture_frame frame{};

  ltr_int_mac_capture_stub_init(&stub);
  ltr_int_mac_capture_stub_submit_frame(&stub, bitmap, 2, 2, 3, 42, 10, 20,
                                        true, false);

  REQUIRE(stub.provider.vtable->next_frame(stub.provider.ctx, &frame) == 1);
  CHECK(frame.gray_bitmap == bitmap);
  CHECK(frame.width == 2);
  CHECK(frame.height == 2);
  CHECK(frame.expected_blobs == 3);
  CHECK(frame.threshold == 42);
  CHECK(frame.min_blob_pixels == 10);
  CHECK(frame.max_blob_pixels == 20);
  CHECK(frame.flip);
  CHECK_FALSE(frame.face_tracking);

  CHECK(stub.provider.vtable->next_frame(stub.provider.ctx, &frame) == 0);
}

TEST_CASE("mac capture stub can queue face-tracking frames",
          "[mac_capture_stub]") {
  static const unsigned char bitmap[] = {9, 8, 7, 6};
  ltr_mac_capture_stub stub{};
  ltr_gray_capture_frame frame{};

  ltr_int_mac_capture_stub_init(&stub);
  ltr_int_mac_capture_stub_submit_frame(&stub, bitmap, 4, 1, 1, 0, 0, 0,
                                        false, true);

  REQUIRE(stub.provider.vtable->next_frame(stub.provider.ctx, &frame) == 1);
  CHECK(frame.gray_bitmap == bitmap);
  CHECK(frame.face_tracking);
  CHECK_FALSE(frame.flip);
}
