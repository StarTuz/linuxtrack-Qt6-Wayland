#include "catch2/catch_amalgamated.hpp"

#include <cmath>

extern "C" {
#include "../tracking.h"
#include "../math_utils.h"
#include "../pose.h"
}

namespace {

bool g_model_changed = true;
bool g_single_point = false;
bool g_face = false;
bool g_absolute = false;
bool g_pose_process_ok = true;
bool g_do_tr_align = false;
int g_orientation = 0;
int g_pose_process_calls = 0;

linuxtrack_pose_t g_stub_pose{};
linuxtrack_abs_pose_t g_stub_abs_pose{};
bloblist_type g_last_pose_blobs{};
blob_type g_last_pose_blob_storage[3]{};

void reset_tracking_stubs() {
  g_model_changed = true;
  g_single_point = false;
  g_face = false;
  g_absolute = false;
  g_pose_process_ok = true;
  g_do_tr_align = false;
  g_orientation = 0;
  g_pose_process_calls = 0;
  g_stub_pose = {};
  g_stub_abs_pose = {};
  g_last_pose_blobs.num_blobs = 0;
  g_last_pose_blobs.expected_blobs = 3;
  g_last_pose_blobs.blobs = g_last_pose_blob_storage;
  for (blob_type &blob : g_last_pose_blob_storage) {
    blob = {};
  }
}

struct owned_frame {
  frame_type frame{};
  blob_type blobs[3]{};
};

owned_frame make_frame(unsigned int width, unsigned int height, int usec,
                       std::initializer_list<blob_type> blobs_in) {
  owned_frame result;
  unsigned int index = 0;
  for (const blob_type &blob : blobs_in) {
    result.blobs[index++] = blob;
  }
  result.frame.width = width;
  result.frame.height = height;
  result.frame.usec = usec;
  result.frame.bloblist.blobs = result.blobs;
  result.frame.bloblist.num_blobs = index;
  result.frame.bloblist.expected_blobs = 3;
  return result;
}

} // namespace

extern "C" {

bool ltr_int_model_changed(bool reset_flag) {
  const bool current = g_model_changed;
  if (reset_flag) {
    g_model_changed = false;
  }
  return current;
}

bool ltr_int_get_model_setup(reflector_model_type *rm) {
  if (rm == nullptr) {
    return false;
  }
  rm->type = static_cast<decltype(rm->type)>(g_single_point ? 2 : 0);
  return true;
}

bool ltr_int_pose_init(struct reflector_model_type /*rm*/) { return true; }

int ltr_int_get_orientation() { return g_orientation; }

void ltr_int_pose_sort_blobs(struct bloblist_type /*bl*/) {}

bool ltr_int_pose_process_blobs(struct bloblist_type blobs,
                                linuxtrack_pose_t *pose,
                                linuxtrack_abs_pose_t *abs_pose,
                                bool /*centering*/) {
  ++g_pose_process_calls;
  g_last_pose_blobs.num_blobs = blobs.num_blobs;
  for (unsigned int i = 0; i < blobs.num_blobs && i < 3; ++i) {
    g_last_pose_blob_storage[i] = blobs.blobs[i];
  }
  if (!g_pose_process_ok) {
    return false;
  }
  if (pose != nullptr) {
    *pose = g_stub_pose;
  }
  if (abs_pose != nullptr) {
    *abs_pose = g_stub_abs_pose;
  }
  return true;
}

bool ltr_int_is_single_point() { return g_single_point; }

bool ltr_int_is_face() { return g_face; }

bool ltr_int_is_absolute() { return g_absolute; }

float ltr_int_val_on_axis(ltr_axes_t /*axes*/, enum axis_t /*id*/, float x) {
  return x;
}

float ltr_int_filter_axis(ltr_axes_t /*axes*/, enum axis_t /*id*/, float x,
                          float *y_minus_1, float /*dt*/) {
  if (y_minus_1 != nullptr) {
    *y_minus_1 = x;
  }
  return x;
}

bool ltr_int_do_tr_align() { return g_do_tr_align; }

} // extern "C"

TEST_CASE("single-point face tracking derives tz from face size changes",
          "[tracking]") {
  reset_tracking_stubs();
  g_single_point = true;
  g_face = true;

  REQUIRE(ltr_int_init_tracking());

  auto recenter_frame =
      make_frame(100, 100, 1000, {{0.0f, 0.0f, 100}});
  auto baseline_frame =
      make_frame(100, 100, 2000, {{0.0f, 0.0f, 100}});
  auto moved_frame =
      make_frame(100, 100, 3000, {{0.0f, 0.0f, 400}});

  REQUIRE(ltr_int_update_pose(&recenter_frame.frame) == -1);
  REQUIRE(ltr_int_update_pose(&baseline_frame.frame) == 0);
  REQUIRE(ltr_int_update_pose(&moved_frame.frame) == 0);

  linuxtrack_full_pose_t full_pose{};
  REQUIRE(ltr_int_tracking_get_pose(&full_pose) == 0);
  CHECK(full_pose.pose.raw_tz == Catch::Approx(-500.0f));
  CHECK(full_pose.timestamp == 3000);
}

TEST_CASE("face tracking with three blobs uses absolute pose path, not 3pt solver",
          "[tracking]") {
  reset_tracking_stubs();
  g_face = true;

  REQUIRE(ltr_int_init_tracking());

  auto recenter_frame = make_frame(
      320, 240, 3000,
      {{1.0f, 2.0f, 10}, {3.0f, 4.0f, 20}, {5.0f, 6.0f, 30}});
  auto steady_frame = make_frame(
      320, 240, 4000,
      {{10.0f, 20.0f, 10}, {30.0f, 40.0f, 20}, {50.0f, 60.0f, 30}});

  REQUIRE(ltr_int_update_pose(&recenter_frame.frame) == -1);
  REQUIRE(ltr_int_update_pose(&steady_frame.frame) == 0);

  linuxtrack_full_pose_t full_pose{};
  REQUIRE(ltr_int_tracking_get_pose(&full_pose) == 0);

  CHECK(g_pose_process_calls == 0);
  CHECK(full_pose.abs_pose.abs_pitch == Catch::Approx(20.0f));
  CHECK(full_pose.abs_pose.abs_yaw == Catch::Approx(10.0f));
  CHECK(full_pose.abs_pose.abs_roll == Catch::Approx(30.0f));
  CHECK(full_pose.abs_pose.abs_tx == Catch::Approx(40.0f));
  CHECK(full_pose.abs_pose.abs_ty == Catch::Approx(50.0f));
  CHECK(full_pose.abs_pose.abs_tz == Catch::Approx(60.0f));
}

TEST_CASE("3-point tracking failure does not advance pose counter", "[tracking]") {
  reset_tracking_stubs();
  g_pose_process_ok = false;

  REQUIRE(ltr_int_init_tracking());

  linuxtrack_full_pose_t before{};
  REQUIRE(ltr_int_tracking_get_pose(&before) == 0);

  auto frame = make_frame(
      640, 480, 5000,
      {{-10.0f, 15.0f, 10}, {0.0f, -15.0f, 20}, {12.0f, 18.0f, 30}});

  REQUIRE(ltr_int_update_pose(&frame.frame) == -1);

  linuxtrack_full_pose_t after{};
  REQUIRE(ltr_int_tracking_get_pose(&after) == 0);

  CHECK(g_pose_process_calls == 1);
  CHECK(after.pose.counter == before.pose.counter);
}

TEST_CASE("camera orientation is normalized before 3-point pose solving",
          "[tracking]") {
  reset_tracking_stubs();
  g_orientation = ORIENT_XCHG_XY | ORIENT_FLIP_X;

  REQUIRE(ltr_int_init_tracking());

  auto frame = make_frame(
      640, 480, 6000,
      {{10.0f, 20.0f, 10}, {-30.0f, 40.0f, 20}, {50.0f, -60.0f, 30}});

  REQUIRE(ltr_int_update_pose(&frame.frame) == -1);
  REQUIRE(g_pose_process_calls == 1);
  REQUIRE(g_last_pose_blobs.num_blobs == 3);

  CHECK(g_last_pose_blobs.blobs[0].x == Catch::Approx(-20.0f));
  CHECK(g_last_pose_blobs.blobs[0].y == Catch::Approx(10.0f));
  CHECK(g_last_pose_blobs.blobs[1].x == Catch::Approx(-40.0f));
  CHECK(g_last_pose_blobs.blobs[1].y == Catch::Approx(-30.0f));
  CHECK(g_last_pose_blobs.blobs[2].x == Catch::Approx(60.0f));
  CHECK(g_last_pose_blobs.blobs[2].y == Catch::Approx(50.0f));
}

TEST_CASE("camera orientation helper rotates a single point consistently",
          "[tracking]") {
  float x = 10.0f;
  float y = 20.0f;

  ltr_int_rotate_camera_point(&x, &y, ORIENT_XCHG_XY | ORIENT_FLIP_X);

  CHECK(x == Catch::Approx(-20.0f));
  CHECK(y == Catch::Approx(10.0f));
}

TEST_CASE("camera orientation helper normalizes an entire bloblist",
          "[tracking]") {
  blob_type blobs[2] = {
      {10.0f, 20.0f, 1},
      {-30.0f, 40.0f, 2},
  };
  bloblist_type bloblist{};
  bloblist.num_blobs = 2;
  bloblist.expected_blobs = 2;
  bloblist.blobs = blobs;

  ltr_int_normalize_bloblist_for_camera_orientation(
      bloblist, ORIENT_XCHG_XY | ORIENT_FLIP_X);

  CHECK(bloblist.blobs[0].x == Catch::Approx(-20.0f));
  CHECK(bloblist.blobs[0].y == Catch::Approx(10.0f));
  CHECK(bloblist.blobs[1].x == Catch::Approx(-40.0f));
  CHECK(bloblist.blobs[1].y == Catch::Approx(-30.0f));
}

TEST_CASE("behind orientation flips shared 3-point pose signs", "[tracking]") {
  reset_tracking_stubs();
  g_orientation = ORIENT_FROM_BEHIND;
  g_stub_pose.raw_pitch = 1.0f;
  g_stub_pose.raw_yaw = 2.0f;
  g_stub_pose.raw_roll = 3.0f;
  g_stub_pose.raw_tx = 4.0f;
  g_stub_pose.raw_ty = 5.0f;
  g_stub_pose.raw_tz = 6.0f;

  REQUIRE(ltr_int_init_tracking());

  auto frame = make_frame(
      640, 480, 7000,
      {{-10.0f, 15.0f, 10}, {0.0f, -15.0f, 20}, {12.0f, 18.0f, 30}});

  REQUIRE(ltr_int_update_pose(&frame.frame) == -1);

  linuxtrack_full_pose_t pose{};
  REQUIRE(ltr_int_tracking_get_pose(&pose) == 0);
  CHECK(pose.pose.raw_pitch == Catch::Approx(-1.0f));
  CHECK(pose.pose.raw_yaw == Catch::Approx(2.0f));
  CHECK(pose.pose.raw_roll == Catch::Approx(-3.0f));
  CHECK(pose.pose.raw_tx == Catch::Approx(-4.0f));
  CHECK(pose.pose.raw_ty == Catch::Approx(5.0f));
  CHECK(pose.pose.raw_tz == Catch::Approx(-6.0f));
}

TEST_CASE("postprocess passes translations through unchanged when alignment is disabled",
          "[tracking]") {
  reset_tracking_stubs();

  linuxtrack_pose_t raw_pose{};
  raw_pose.raw_pitch = 0.0f;
  raw_pose.raw_yaw = 0.0f;
  raw_pose.raw_roll = 0.0f;
  raw_pose.raw_tx = 10.0f;
  raw_pose.raw_ty = -20.0f;
  raw_pose.raw_tz = 30.0f;

  linuxtrack_pose_t filtered_pose = raw_pose;
  linuxtrack_pose_t unfiltered{};

  REQUIRE(ltr_int_postprocess_axes(nullptr, &filtered_pose, &unfiltered));
  CHECK(unfiltered.tx == Catch::Approx(10.0f));
  CHECK(unfiltered.ty == Catch::Approx(-20.0f));
  CHECK(unfiltered.tz == Catch::Approx(30.0f));
  CHECK(filtered_pose.tx == Catch::Approx(10.0f));
  CHECK(filtered_pose.ty == Catch::Approx(-20.0f));
  CHECK(filtered_pose.tz == Catch::Approx(30.0f));
}

TEST_CASE("postprocess rotates translations when alignment is enabled",
          "[tracking]") {
  reset_tracking_stubs();
  g_do_tr_align = true;

  linuxtrack_pose_t raw_pose{};
  raw_pose.raw_pitch = 0.0f;
  raw_pose.raw_yaw = 90.0f;
  raw_pose.raw_roll = 0.0f;
  raw_pose.raw_tx = 0.0f;
  raw_pose.raw_ty = 0.0f;
  raw_pose.raw_tz = 10.0f;

  linuxtrack_pose_t filtered_pose = raw_pose;
  linuxtrack_pose_t unfiltered{};

  REQUIRE(ltr_int_postprocess_axes(nullptr, &filtered_pose, &unfiltered));
  CHECK(unfiltered.tx == Catch::Approx(10.0f).margin(0.001));
  CHECK(unfiltered.ty == Catch::Approx(0.0f).margin(0.001));
  CHECK(unfiltered.tz == Catch::Approx(0.0f).margin(0.001));
  CHECK(filtered_pose.tx == Catch::Approx(10.0f).margin(0.001));
  CHECK(filtered_pose.ty == Catch::Approx(0.0f).margin(0.001));
  CHECK(filtered_pose.tz == Catch::Approx(0.0f).margin(0.001));
}

TEST_CASE("postprocess rejects non-finite axis values", "[tracking]") {
  reset_tracking_stubs();

  linuxtrack_pose_t raw_pose{};
  raw_pose.raw_pitch = NAN;
  raw_pose.raw_yaw = 0.0f;
  raw_pose.raw_roll = 0.0f;
  raw_pose.raw_tx = 0.0f;
  raw_pose.raw_ty = 0.0f;
  raw_pose.raw_tz = 0.0f;

  linuxtrack_pose_t filtered_pose = raw_pose;
  linuxtrack_pose_t unfiltered{};

  REQUIRE_FALSE(ltr_int_postprocess_axes(nullptr, &filtered_pose, &unfiltered));
}
