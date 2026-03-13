#include "catch2/catch_amalgamated.hpp"

extern "C" {
#include "../capture_process.h"
}

namespace {

int g_to_stripes_calls = 0;
int g_to_blobs_calls = 0;
int g_face_detect_calls = 0;
bloblist_type *g_last_bloblist = nullptr;

void reset_capture_process_stubs() {
  g_to_stripes_calls = 0;
  g_to_blobs_calls = 0;
  g_face_detect_calls = 0;
  g_last_bloblist = nullptr;
}

} // namespace

extern "C" {

void ltr_int_to_stripes(image_t * /*img*/) { ++g_to_stripes_calls; }

int ltr_int_stripes_to_blobs(unsigned int /*num_blobs*/,
                             struct bloblist_type *blt, int /*min_pts*/,
                             int /*max_pts*/, image_t * /*img*/) {
  ++g_to_blobs_calls;
  g_last_bloblist = blt;
  blt->num_blobs = 1;
  blt->blobs[0].x = 10.0f;
  blt->blobs[0].y = -20.0f;
  blt->blobs[0].score = 123;
  return 0;
}

void ltr_int_face_detect(image_t * /*img*/, struct bloblist_type *blt) {
  ++g_face_detect_calls;
  g_last_bloblist = blt;
  blt->num_blobs = 1;
  blt->blobs[0].x = 1.0f;
  blt->blobs[0].y = 2.0f;
  blt->blobs[0].score = 3;
}

} // extern "C"

TEST_CASE("capture process routes non-face frames through blob extraction",
          "[capture_process]") {
  reset_capture_process_stubs();
  blob_type blobs[3] = {};
  bloblist_type bloblist{.num_blobs = 0, .expected_blobs = 3, .blobs = blobs};
  unsigned char bitmap[4] = {};
  image_t img{.w = 2, .h = 2, .bitmap = bitmap, .ratio = 1.0f};

  REQUIRE(ltr_int_process_gray_image(&img, &bloblist, 10, 20, true, false) ==
          0);

  CHECK(g_to_stripes_calls == 1);
  CHECK(g_to_blobs_calls == 1);
  CHECK(g_face_detect_calls == 0);
  CHECK(g_last_bloblist == &bloblist);
  CHECK(bloblist.blobs[0].x == Catch::Approx(-10.0f));
  CHECK(bloblist.blobs[0].y == Catch::Approx(20.0f));
  CHECK(bloblist.blobs[0].score == 123);
}

TEST_CASE("capture process routes face frames through face detector",
          "[capture_process]") {
  reset_capture_process_stubs();
  blob_type blobs[3] = {};
  bloblist_type bloblist{.num_blobs = 0, .expected_blobs = 3, .blobs = blobs};
  unsigned char bitmap[4] = {};
  image_t img{.w = 2, .h = 2, .bitmap = bitmap, .ratio = 1.0f};

  REQUIRE(ltr_int_process_gray_image(&img, &bloblist, 10, 20, true, true) ==
          0);

  CHECK(g_to_stripes_calls == 0);
  CHECK(g_to_blobs_calls == 0);
  CHECK(g_face_detect_calls == 1);
  CHECK(g_last_bloblist == &bloblist);
  CHECK(bloblist.blobs[0].x == Catch::Approx(1.0f));
  CHECK(bloblist.blobs[0].y == Catch::Approx(2.0f));
  CHECK(bloblist.blobs[0].score == 3);
}
