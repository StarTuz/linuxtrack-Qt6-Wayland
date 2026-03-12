#include "catch2/catch_amalgamated.hpp"

#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>

extern "C" {
#include "../pref_global.h"
}

namespace {

using SectionMap = std::unordered_map<std::string, std::string>;

std::unordered_map<std::string, SectionMap> prefs;

char *dup_cstr(const std::string &value) {
  char *copy = static_cast<char *>(std::malloc(value.size() + 1));
  if (copy != nullptr) {
    std::memcpy(copy, value.c_str(), value.size() + 1);
  }
  return copy;
}

void reset_prefs() {
  prefs.clear();
  prefs["Global"]["Input"] = "Device";
}

void set_device(const char *type, const char *id) {
  prefs["Device"]["Capture-device"] = type;
  prefs["Device"]["Capture-device-id"] = id;
}

} // namespace

extern "C" {

void ltr_int_free_prefs() {}

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

void ltr_int_log_message(const char *format, ...) { (void)format; }

} // extern "C"

TEST_CASE("pref_global maps legacy mac capture device types to shared categories",
          "[pref_global]") {
  struct {
    const char *device_type;
    cal_device_category_type expected_category;
  } cases[] = {
      {"MacWebcam", mac_webcam},
      {"MacWebcam-face", mac_webcam_ft},
      {"Ps3Eye", mac_ps3eye},
      {"Ps3Eye-face", mac_ps3eye_ft},
  };

  for (const auto &tc : cases) {
    reset_prefs();
    set_device(tc.device_type, "camera-1");

    camera_control_block ccb{};
    REQUIRE(ltr_int_get_device(&ccb));
    CHECK(ccb.device.category == tc.expected_category);
    REQUIRE(ccb.device.device_id != nullptr);
    CHECK(std::string(ccb.device.device_id) == "camera-1");
    std::free(ccb.device.device_id);
  }
}

TEST_CASE("pref_global rejects unknown capture device types", "[pref_global]") {
  reset_prefs();
  set_device("DefinitelyNotADevice", "mystery");

  camera_control_block ccb{};
  REQUIRE_FALSE(ltr_int_get_device(&ccb));
}

TEST_CASE("pref_global camera orientation defaults to zero", "[pref_global]") {
  reset_prefs();
  CHECK(ltr_int_get_orientation() == 0);

  prefs["Global"]["Camera-orientation"] = "12";
  CHECK(ltr_int_get_orientation() == 12);
}
