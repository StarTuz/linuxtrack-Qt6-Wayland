// Regression tests for webcam_info.cpp static methods.
// Covers bugs fixed in the "useless dropdown" and "Y driver-dependent" incidents.
//
// WebcamLibProxy calls ltr_int_load_library at static-init time; the stub in
// stubs_dyn_load.cpp makes it return nullptr so webcamInfoOk stays false.
// The static methods under test do not require a live webcam library.

#include "catch2/catch_amalgamated.hpp"
#include "../qt_gui/webcam_info.h"

// -------------------------------------------------------------------------
// describeFormatPolicy — regression for isNativeFormat / isCompressedFormat
// -------------------------------------------------------------------------

TEST_CASE("describeFormatPolicy - native formats say Native", "[webcam_info]") {
  CHECK(WebcamInfo::describeFormatPolicy("YUYV", false).contains("Native"));
  CHECK(WebcamInfo::describeFormatPolicy("YU12", false).contains("Native"));
  CHECK(WebcamInfo::describeFormatPolicy("YV12", false).contains("Native"));
  CHECK(WebcamInfo::describeFormatPolicy("RGB3", false).contains("Native"));
  CHECK(WebcamInfo::describeFormatPolicy("BGR3", false).contains("Native"));
  CHECK(WebcamInfo::describeFormatPolicy("GREY", false).contains("Native"));
}

TEST_CASE("describeFormatPolicy - GREY is Native not Driver-dependent",
          "[webcam_info]") {
  // Regression: before the fix GREY was not in isNativeFormat() and returned
  // "Driver-dependent".
  const QString desc = WebcamInfo::describeFormatPolicy("GREY", false);
  CHECK(desc.contains("Native"));
  CHECK_FALSE(desc.contains("Driver-dependent"));
}

TEST_CASE("describeFormatPolicy - compressed formats say Compressed",
          "[webcam_info]") {
  // describeFormatPolicy uses "Compressed camera mode"; getFormatDisplayName
  // uses "(Converted)" — these are different functions.
  CHECK(WebcamInfo::describeFormatPolicy("MJPG", false).contains("Compressed"));
  CHECK(WebcamInfo::describeFormatPolicy("JPEG", false).contains("Compressed"));
}

TEST_CASE("describeFormatPolicy - unknown format says Driver-dependent",
          "[webcam_info]") {
  CHECK(WebcamInfo::describeFormatPolicy("NV12", false).contains("Driver-dependent"));
}

TEST_CASE("describeFormatPolicy - single-char Y is not Native",
          "[webcam_info]") {
  // Regression: the broken kYUYV fallback (*"YUYV" == 0x59 == 'Y') caused
  // getFourcc() to return "Y" for the default format.  Ensure "Y" is never
  // mistakenly classified as native.
  const QString desc = WebcamInfo::describeFormatPolicy("Y", false);
  CHECK_FALSE(desc.contains("Native"));
  CHECK(desc.contains("Driver-dependent"));
}

// -------------------------------------------------------------------------
// decodeRes — regex parsing
// -------------------------------------------------------------------------

TEST_CASE("decodeRes parses standard resolution string", "[webcam_info]") {
  int x = 0, y = 0;
  REQUIRE(WebcamInfo::decodeRes("640 x 480", x, y));
  CHECK(x == 640);
  CHECK(y == 480);
}

TEST_CASE("decodeRes parses large resolution", "[webcam_info]") {
  int x = 0, y = 0;
  REQUIRE(WebcamInfo::decodeRes("1280 x 720", x, y));
  CHECK(x == 1280);
  CHECK(y == 720);
}

TEST_CASE("decodeRes parses uppercase X separator", "[webcam_info]") {
  int x = 0, y = 0;
  REQUIRE(WebcamInfo::decodeRes("320X240", x, y));
  CHECK(x == 320);
  CHECK(y == 240);
}

TEST_CASE("decodeRes rejects garbage input", "[webcam_info]") {
  int x = 0, y = 0;
  CHECK_FALSE(WebcamInfo::decodeRes("not a resolution", x, y));
  CHECK_FALSE(WebcamInfo::decodeRes("640", x, y));
  CHECK_FALSE(WebcamInfo::decodeRes("", x, y));
}

// -------------------------------------------------------------------------
// decodeFps — rational fps parsing
// -------------------------------------------------------------------------

TEST_CASE("decodeFps parses num/den fps strings", "[webcam_info]") {
  int num = 0, den = 0;
  REQUIRE(WebcamInfo::decodeFps("1/30", num, den));
  CHECK(num == 1);
  CHECK(den == 30);
}

TEST_CASE("decodeFps parses with spaces around slash", "[webcam_info]") {
  int num = 0, den = 0;
  REQUIRE(WebcamInfo::decodeFps("1 / 60", num, den));
  CHECK(num == 1);
  CHECK(den == 60);
}

TEST_CASE("decodeFps rejects garbage input", "[webcam_info]") {
  int num = 0, den = 0;
  CHECK_FALSE(WebcamInfo::decodeFps("30", num, den));
  CHECK_FALSE(WebcamInfo::decodeFps("not fps", num, den));
  CHECK_FALSE(WebcamInfo::decodeFps("", num, den));
}
