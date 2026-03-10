// Stubs for dynamic library loading so webcam_info.cpp compiles and links in
// test binaries without the actual runtime libraries present.

extern "C" {
#include "../dyn_load.h"
#include "../list.h"

void *ltr_int_load_library(char * /*lib_name*/, lib_fun_def_t *func_defs) {
  // Simulate library not found: leave all function pointers NULL.
  // webcamInfoOk will remain false; static methods on WebcamInfo still work.
  (void)func_defs;
  return nullptr;
}

int ltr_int_unload_library(void * /*libhandle*/, lib_fun_def_t * /*func_defs*/) {
  return 0;
}

void ltr_int_array_cleanup(char ***arr) {
  (void)arr;
}

} // extern "C"
