#include "catch2/catch_amalgamated.hpp"

extern "C" {
#include "../frame_adapter.h"
}

namespace {

unsigned char rgb_to_y(unsigned char r, unsigned char g, unsigned char b) {
  float y = 0.257f * static_cast<float>(r) +
            0.504f * static_cast<float>(g) +
            0.098f * static_cast<float>(b) + 16.0f;
  if (y > 255.0f) {
    y = 255.0f;
  }
  return static_cast<unsigned char>(y);
}

} // namespace

TEST_CASE("fourcc helper round-trips printable tags", "[frame_adapter]") {
  char out[5]{};
  const uint32_t fourcc = ltr_int_frame_fourcc("YUYV");

  ltr_int_fourcc_to_string(fourcc, out);

  CHECK(std::string(out) == "YUYV");
}

TEST_CASE("YUYV conversion copies luma bytes into grayscale output",
          "[frame_adapter]") {
  const unsigned char source[] = {10, 200, 20, 201, 30, 202, 40, 203};
  unsigned char dest[4] = {0, 0, 0, 0};

  REQUIRE(ltr_int_convert_frame_to_gray(ltr_int_frame_fourcc("YUYV"), 2, 2,
                                        source, sizeof(source), dest));

  CHECK(dest[0] == 10);
  CHECK(dest[1] == 20);
  CHECK(dest[2] == 30);
  CHECK(dest[3] == 40);
}

TEST_CASE("RGB3 conversion follows shared luminance formula",
          "[frame_adapter]") {
  const unsigned char source[] = {
      255, 0, 0,
      0, 255, 0,
      0, 0, 255,
      255, 255, 255,
  };
  unsigned char dest[4] = {0, 0, 0, 0};

  REQUIRE(ltr_int_convert_frame_to_gray(ltr_int_frame_fourcc("RGB3"), 2, 2,
                                        source, sizeof(source), dest));

  CHECK(dest[0] == rgb_to_y(255, 0, 0));
  CHECK(dest[1] == rgb_to_y(0, 255, 0));
  CHECK(dest[2] == rgb_to_y(0, 0, 255));
  CHECK(dest[3] == rgb_to_y(255, 255, 255));
}

TEST_CASE("BGR3 conversion interprets source channels in reverse order",
          "[frame_adapter]") {
  const unsigned char source[] = {
      0, 0, 255,
      0, 255, 0,
      255, 0, 0,
      255, 255, 255,
  };
  unsigned char dest[4] = {0, 0, 0, 0};

  REQUIRE(ltr_int_convert_frame_to_gray(ltr_int_frame_fourcc("BGR3"), 2, 2,
                                        source, sizeof(source), dest));

  CHECK(dest[0] == rgb_to_y(255, 0, 0));
  CHECK(dest[1] == rgb_to_y(0, 255, 0));
  CHECK(dest[2] == rgb_to_y(0, 0, 255));
  CHECK(dest[3] == rgb_to_y(255, 255, 255));
}

TEST_CASE("planar and gray formats zero-fill incomplete frames",
          "[frame_adapter]") {
  const unsigned char source[] = {9, 8, 7};
  unsigned char dest[4] = {1, 1, 1, 1};

  REQUIRE(ltr_int_convert_frame_to_gray(ltr_int_frame_fourcc("GREY"), 2, 2,
                                        source, sizeof(source), dest));

  CHECK(dest[0] == 9);
  CHECK(dest[1] == 8);
  CHECK(dest[2] == 7);
  CHECK(dest[3] == 0);
}

TEST_CASE("unsupported formats report failure and black out the frame",
          "[frame_adapter]") {
  const unsigned char source[] = {1, 2, 3, 4};
  unsigned char dest[4] = {9, 9, 9, 9};

  CHECK_FALSE(ltr_int_convert_frame_to_gray(ltr_int_frame_fourcc("ZZZZ"), 2, 2,
                                            source, sizeof(source), dest));

  CHECK(dest[0] == 0);
  CHECK(dest[1] == 0);
  CHECK(dest[2] == 0);
  CHECK(dest[3] == 0);
}

TEST_CASE("frame clock stamps the first acquired frame",
          "[frame_adapter]") {
  frame_type frame{};
  ltr_frame_clock_state state{};

  ltr_int_reset_frame_clock(&state);

  REQUIRE(ltr_int_finalize_capture_frame(&frame, &state, true, 1234));
  CHECK(frame.counter == 1);
  CHECK(frame.usec == 1234);
}

TEST_CASE("frame clock ignores dropped frames",
          "[frame_adapter]") {
  frame_type frame{};
  ltr_frame_clock_state state{};

  ltr_int_reset_frame_clock(&state);

  CHECK_FALSE(ltr_int_finalize_capture_frame(&frame, &state, false, 1234));
  CHECK(state.counter == 0);
  CHECK_FALSE(state.initialized);

  REQUIRE(ltr_int_finalize_capture_frame(&frame, &state, true, 2000));
  CHECK(frame.counter == 1);
  CHECK(frame.usec == 2000);
}

TEST_CASE("frame clock bumps duplicate and backward timestamps",
          "[frame_adapter]") {
  frame_type frame{};
  ltr_frame_clock_state state{};

  ltr_int_reset_frame_clock(&state);

  REQUIRE(ltr_int_finalize_capture_frame(&frame, &state, true, 5000));
  CHECK(frame.usec == 5000);
  REQUIRE(ltr_int_finalize_capture_frame(&frame, &state, true, 5000));
  CHECK(frame.counter == 2);
  CHECK(frame.usec == 5001);
  REQUIRE(ltr_int_finalize_capture_frame(&frame, &state, true, 4990));
  CHECK(frame.counter == 3);
  CHECK(frame.usec == 5002);
}

TEST_CASE("frame clock preserves wraparound-sized jumps",
          "[frame_adapter]") {
  frame_type frame{};
  ltr_frame_clock_state state{};
  const int wrap_usecs = 1024 * 1000000;

  ltr_int_reset_frame_clock(&state);

  REQUIRE(ltr_int_finalize_capture_frame(&frame, &state, true,
                                         wrap_usecs - 5));
  REQUIRE(ltr_int_finalize_capture_frame(&frame, &state, true, 3));
  CHECK(frame.counter == 2);
  CHECK(frame.usec == 3);
}
