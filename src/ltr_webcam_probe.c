#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cal.h"
#include "list.h"
#include "pref.h"
#include "pref_global.h"
#include "runloop.h"
#include "utils.h"
#include "wc_driver_prefs.h"
#include "webcam_driver.h"

typedef struct {
  const char *device_id;
  const char *pixel_format;
  int width;
  int height;
  int fps_num;
  int fps_den;
  int frames;
} probe_config;

static void print_usage(const char *argv0) {
  printf("Usage:\n");
  printf("  %s --list\n", argv0);
  printf("  %s --device <id> [--format <fourcc>] [--size <WxH>] [--fps <num/den>] [--frames <n>]\n",
         argv0);
}

static bool parse_size(const char *arg, int *width, int *height) {
  return sscanf(arg, "%dx%d", width, height) == 2 ||
         sscanf(arg, "%dX%d", width, height) == 2;
}

static bool parse_fps(const char *arg, int *num, int *den) {
  return sscanf(arg, "%d/%d", num, den) == 2;
}

static int list_webcams(void) {
  char **webcams = NULL;
  int count = ltr_int_enum_webcams(&webcams);
  if (count <= 0 || webcams == NULL) {
    printf("No webcams found.\n");
    return 0;
  }

  for (int i = 0; webcams[i] != NULL; ++i) {
    webcam_formats formats;
    printf("%s\n", webcams[i]);
    if (ltr_int_enum_webcam_formats(webcams[i], &formats) > 0) {
      for (int j = 0; j < formats.entries; ++j) {
        webcam_format *fmt = &formats.formats[j];
        printf("  %c%c%c%c %dx%d @ %.1f fps\n", fmt->fourcc & 0xFF,
               (fmt->fourcc >> 8) & 0xFF, (fmt->fourcc >> 16) & 0xFF,
               (fmt->fourcc >> 24) & 0xFF, fmt->w, fmt->h,
               (fmt->fps_num > 0) ? ((float)fmt->fps_den / (float)fmt->fps_num)
                                  : 0.0f);
      }
      ltr_int_enum_webcam_formats_cleanup(&formats);
    }
  }

  ltr_int_array_cleanup(&webcams);
  return 0;
}

static bool setup_probe_prefs(const probe_config *cfg) {
  char resolution[64];
  char fps[64];

  if (!ltr_int_new_prefs()) {
    return false;
  }
  snprintf(resolution, sizeof(resolution), "%d x %d", cfg->width, cfg->height);
  snprintf(fps, sizeof(fps), "%d/%d", cfg->fps_num, cfg->fps_den);
  return ltr_int_change_key("Global", "Input", "ProbeWebcam") &&
         ltr_int_change_key("ProbeWebcam", "Capture-device", "Webcam") &&
         ltr_int_change_key("ProbeWebcam", "Capture-device-id", cfg->device_id) &&
         ltr_int_change_key("ProbeWebcam", "Pixel-format", cfg->pixel_format) &&
         ltr_int_change_key("ProbeWebcam", "Resolution", resolution) &&
         ltr_int_change_key("ProbeWebcam", "Fps", fps) &&
         ltr_int_change_key_int("ProbeWebcam", "Threshold", 140) &&
         ltr_int_change_key_int("ProbeWebcam", "Min-blob", 4) &&
         ltr_int_change_key_int("ProbeWebcam", "Max-blob", 230) &&
         ltr_int_change_key("ProbeWebcam", "Upside-down", "No");
}

static int probe_webcam(const probe_config *cfg) {
  struct camera_control_block ccb;
  struct frame_type frame;
  bool frame_acquired = false;

  memset(&ccb, 0, sizeof(ccb));
  memset(&frame, 0, sizeof(frame));

  if (!setup_probe_prefs(cfg)) {
    fprintf(stderr, "Failed to create temporary probe preferences.\n");
    return 1;
  }

  ccb.device.category = webcam;
  ccb.device.device_id = ltr_int_my_strdup(cfg->device_id);
  frame.bloblist.blobs =
      ltr_int_my_malloc(sizeof(struct blob_type) * MAX_BLOBS);
  frame.bloblist.num_blobs = MAX_BLOBS;
  frame.bloblist.expected_blobs = 3;

  if (ltr_int_tracker_init(&ccb) != 0) {
    fprintf(stderr, "Probe init failed for '%s'.\n", cfg->device_id);
    free(ccb.device.device_id);
    ltr_int_frame_free(&ccb, &frame);
    ltr_int_close_prefs();
    return 1;
  }

  printf("Probe started: %s\n", cfg->device_id);
  printf("Requested: %s %dx%d @ %.1f fps\n", cfg->pixel_format, cfg->width,
         cfg->height, (float)cfg->fps_den / (float)cfg->fps_num);

  for (int i = 0; i < cfg->frames; ++i) {
    frame_acquired = false;
    if (ltr_int_tracker_get_frame(&ccb, &frame, &frame_acquired) != 0) {
      fprintf(stderr, "Frame acquisition failed at frame %d.\n", i + 1);
      break;
    }
    if (!frame_acquired) {
      printf("Frame %d: no frame acquired.\n", i + 1);
      continue;
    }
    printf("Frame %d: %s", i + 1,
           frame.camera_diag[0] != '\0' ? frame.camera_diag
                                        : "No driver diagnostics");
    if (frame.camera_diag2[0] != '\0') {
      printf(" | %s", frame.camera_diag2);
    }
    printf(" | frame=%ux%u blobs=%u\n", frame.width, frame.height,
           frame.bloblist.num_blobs);
  }

  ltr_int_tracker_close();
  free(ccb.device.device_id);
  ltr_int_frame_free(&ccb, &frame);
  ltr_int_close_prefs();
  return 0;
}

int main(int argc, char *argv[]) {
  probe_config cfg = {.device_id = NULL,
                      .pixel_format = "YUYV",
                      .width = 640,
                      .height = 480,
                      .fps_num = 1,
                      .fps_den = 30,
                      .frames = 3};

  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--list") == 0) {
      return list_webcams();
    }
    if ((strcmp(argv[i], "--device") == 0) && (i + 1 < argc)) {
      cfg.device_id = argv[++i];
      continue;
    }
    if ((strcmp(argv[i], "--format") == 0) && (i + 1 < argc)) {
      cfg.pixel_format = argv[++i];
      continue;
    }
    if ((strcmp(argv[i], "--size") == 0) && (i + 1 < argc)) {
      if (!parse_size(argv[++i], &cfg.width, &cfg.height)) {
        fprintf(stderr, "Invalid --size value.\n");
        return 1;
      }
      continue;
    }
    if ((strcmp(argv[i], "--fps") == 0) && (i + 1 < argc)) {
      if (!parse_fps(argv[++i], &cfg.fps_num, &cfg.fps_den)) {
        fprintf(stderr, "Invalid --fps value.\n");
        return 1;
      }
      continue;
    }
    if ((strcmp(argv[i], "--frames") == 0) && (i + 1 < argc)) {
      cfg.frames = atoi(argv[++i]);
      continue;
    }
    print_usage(argv[0]);
    return 1;
  }

  if (cfg.device_id == NULL) {
    fprintf(stderr, "--device is required unless --list is used.\n");
    return 1;
  }

  return probe_webcam(&cfg);
}
