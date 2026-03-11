#include "catch2/catch_amalgamated.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>

extern "C" {
#include "../wc_driver_prefs.h"
}

namespace {

using SectionMap = std::unordered_map<std::string, std::string>;

std::unordered_map<std::string, SectionMap> prefs;
std::string device_section = "Webcam-face";

char *dup_cstr(const std::string &value) {
  char *copy = static_cast<char *>(std::malloc(value.size() + 1));
  if (copy != nullptr) {
    std::memcpy(copy, value.c_str(), value.size() + 1);
  }
  return copy;
}

void reset_prefs(const char *device_type) {
  prefs.clear();
  prefs[device_section]["Capture-device"] = device_type;
}

void set_pref(const char *section, const char *key, const char *value) {
  prefs[section][key] = value;
}

float get_pref_float(const char *section, const char *key) {
  return std::strtof(prefs[section][key].c_str(), nullptr);
}

} // namespace

extern "C" {

char *ltr_int_get_device_section() { return dup_cstr(device_section); }

char *ltr_int_get_key(const char *section_name, const char *key_name) {
  const auto section_it = prefs.find(section_name);
  if (section_it == prefs.end()) {
    return nullptr;
  }
  const auto key_it = section_it->second.find(key_name);
  if (key_it == section_it->second.end()) {
    return nullptr;
  }
  return dup_cstr(key_it->second);
}

bool ltr_int_get_key_flt(const char *section_name, const char *key_name,
                         float *val) {
  char *value = ltr_int_get_key(section_name, key_name);
  if (value == nullptr) {
    return false;
  }
  *val = std::strtof(value, nullptr);
  std::free(value);
  return true;
}

bool ltr_int_get_key_int(const char *section_name, const char *key_name,
                         int *val) {
  char *value = ltr_int_get_key(section_name, key_name);
  if (value == nullptr) {
    return false;
  }
  *val = std::strtol(value, nullptr, 10);
  std::free(value);
  return true;
}

bool ltr_int_change_key(const char *section_name, const char *key_name,
                        const char *new_value) {
  if (new_value == nullptr) {
    prefs[section_name].erase(key_name);
  } else {
    prefs[section_name][key_name] = new_value;
  }
  return true;
}

bool ltr_int_change_key_flt(const char *section_name, const char *key_name,
                            float new_value) {
  char buffer[64];
  std::snprintf(buffer, sizeof(buffer), "%g", new_value);
  return ltr_int_change_key(section_name, key_name, buffer);
}

bool ltr_int_change_key_int(const char *section_name, const char *key_name,
                            int new_value) {
  char buffer[64];
  std::snprintf(buffer, sizeof(buffer), "%d", new_value);
  return ltr_int_change_key(section_name, key_name, buffer);
}

char *ltr_int_get_data_path(const char *data) {
  return dup_cstr(std::string("/definitely-missing/") +
                  ((data != nullptr) ? data : ""));
}

char *ltr_int_my_strdup(const char *s) {
  if (s == nullptr) {
    return nullptr;
  }
  return dup_cstr(s);
}

void ltr_int_log_message(const char *format, ...) { (void)format; }

} // extern "C"

TEST_CASE("face tracker camera fov defaults to upstream value", "[wc_prefs]") {
  reset_prefs("Webcam-face");

  REQUIRE(ltr_int_wc_init_prefs());
  REQUIRE(ltr_int_wc_get_camera_fov() == Catch::Approx(56.0f));
}

TEST_CASE("face tracker camera fov honors saved overrides", "[wc_prefs]") {
  reset_prefs("Webcam-face");
  set_pref("Webcam-face", "Camera-fov", "72.5");

  REQUIRE(ltr_int_wc_init_prefs());
  REQUIRE(ltr_int_wc_get_camera_fov() == Catch::Approx(72.5f));
}

TEST_CASE("face tracker camera fov setter clamps to supported range",
          "[wc_prefs]") {
  reset_prefs("Webcam-face");
  REQUIRE(ltr_int_wc_init_prefs());

  REQUIRE(ltr_int_wc_set_camera_fov(10.0f));
  REQUIRE(ltr_int_wc_get_camera_fov() == Catch::Approx(30.0f));
  REQUIRE(get_pref_float("Webcam-face", "Camera-fov") == Catch::Approx(30.0f));

  REQUIRE(ltr_int_wc_set_camera_fov(160.0f));
  REQUIRE(ltr_int_wc_get_camera_fov() == Catch::Approx(140.0f));
  REQUIRE(get_pref_float("Webcam-face", "Camera-fov") == Catch::Approx(140.0f));
}
