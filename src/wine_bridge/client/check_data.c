#define WIN32_LEAN_AND_MEAN
#include "rest.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>

static char *linuxtrack_config_base(void) {
  const char *xdg_config_home = getenv("XDG_CONFIG_HOME");
  const char *home = getenv("HOME");
  const char *userprofile = getenv("USERPROFILE");

  if ((xdg_config_home != NULL) && (xdg_config_home[0] != '\0')) {
    size_t size = strlen(xdg_config_home) + strlen("linuxtrack") + 3;
    char *path = malloc(size);
    if (path != NULL) {
      snprintf(path, size, "%s/%s", xdg_config_home, "linuxtrack");
    }
    return path;
  }

  if ((home != NULL) && (home[0] != '\0')) {
    size_t size = strlen(home) + strlen("linuxtrack") + 11;
    char *path = malloc(size);
    if (path != NULL) {
      snprintf(path, size, "%s/.config/%s", home, "linuxtrack");
    }
    return path;
  }

  if ((userprofile != NULL) && (userprofile[0] != '\0')) {
    size_t size = strlen(userprofile) + strlen("linuxtrack") + 3;
    char *path = malloc(size);
    if (path != NULL) {
      snprintf(path, size, "%s/%s", userprofile, "linuxtrack");
    }
    return path;
  }

  return strdup(".");
}

int main() {
  // signatures
  char verse1[200], verse2[200];
  game_desc_t gd;
  if (game_data_get_desc(1001, &gd) && getSomeSeriousPoetry(verse1, verse2)) {
    // data available, all is OK
  } else {
    MessageBox(NULL,
               "To fully utilize linuxtrack-wine,\ninstall the support data in "
               "ltr_gui!",
               "Linuxtrack-wine check", MB_OK);
  }
  char *config_base = linuxtrack_config_base();
  if (config_base == NULL) {
    return 0;
  }
  size_t path1_size = 200 + strlen(config_base);
  char *path1 = malloc(path1_size);
  snprintf(path1, path1_size, "%s/tir_firmware/TIRViews.dll", config_base);
  if (symlink(path1, "TIRViews.dll") != 0) {
    MessageBox(NULL,
               "Failed to create symlink to TIRViews.dll!\nSome games will not "
               "have headtracking available.",
               "Linuxtrack-wine check", MB_OK);
  }
  snprintf(path1, path1_size, "%s/tir_firmware/mfc42u.dll", config_base);
  if (symlink(path1, "mfc42u.dll") != 0) {
    MessageBox(NULL,
               "Failed to create symlink to mfc42u.dll!\n"
               "Try to install TIRViews support in ltr_gui,\n"
               "or install mfc42 into this bottle using winetricks.",
               "Linuxtrack-wine check", MB_OK);
  }
  free(config_base);
  free(path1);
  return 0;
}
