#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
#include "../utils.h"
}

extern "C" void *ltr_int_my_malloc(size_t size) {
  void *ptr = std::malloc(size);
  if (ptr == nullptr) {
    std::abort();
  }
  return ptr;
}

extern "C" char *ltr_int_my_strdup(const char *s) {
  if (s == nullptr) {
    return nullptr;
  }
  size_t len = std::strlen(s) + 1;
  char *copy = static_cast<char *>(ltr_int_my_malloc(len));
  std::memcpy(copy, s, len);
  return copy;
}

extern "C" void ltr_int_valog_message(const char *format, va_list va) {
  std::vfprintf(stderr, format, va);
}

extern "C" void ltr_int_log_message(const char *format, ...) {
  va_list va;
  va_start(va, format);
  ltr_int_valog_message(format, va);
  va_end(va);
}

extern "C" dbg_flag_type ltr_int_get_dbg_flag(const int /*flag*/) {
  return DBG_OFF;
}
