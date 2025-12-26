# Linuxtrack Modern Linux Compilation Fixes

## Date: 2025-12-25

## Summary

This document records the minimal, non-architectural changes made to fix Linuxtrack compilation on modern Linux systems (tested on Arch Linux with GCC 14.x, Wine 9.x, Qt 5.x/6.x).

## Issues Fixed

### 1. Missing `<pthread.h>` Include in C++ Files

**Files Modified:**
- `src/ltr_srv_master.cpp`
- `src/pref.cpp`

**Problem:** Modern GCC has stricter header isolation. Previously, `pthread.h` might have been implicitly included through other system headers, but this is no longer the case.

**Error:**
```
ltr_srv_master.cpp:35:34: error: 'PTHREAD_MUTEX_INITIALIZER' was not declared in this scope
```

**Fix:** Added explicit `#include <pthread.h>` to both files.

---

### 2. Removed Wine Extension `DLL_WINE_PREATTACH`

**Files Modified:**
- `src/wine_bridge/client/NPClient_main.c`
- `src/wine_bridge/ft_client/FreeTrackClient_main.c`

**Problem:** `DLL_WINE_PREATTACH` was a Wine-specific internal extension that was removed in Wine 6.0 (February 2021) after the underlying functionality was fully implemented into Wine's core.

**Error:**
```
NPClient_main.c:55:14: error: 'DLL_WINE_PREATTACH' undeclared (first use in this function)
```

**Fix:** Used conditional compilation (`#ifdef DLL_WINE_PREATTACH`) to maintain backward compatibility with older Wine versions while compiling on newer versions.

---

### 3. Wine Debug Channel Name Too Long

**File Modified:**
- `src/wine_bridge/ft_client/FreeTrackClient_main.c`

**Problem:** Wine debug channel names have a 15-character limit (including NUL terminator). "FreeTrackClient" is 15 characters without NUL.

**Error:**
```
FreeTrackClient_main.c:22:1: error: static assertion failed: "sizeof(\"FreeTrackClient\") <= sizeof(__wine_dbch_FreeTrackClient.name)"
```

**Fix:** Shortened the debug channel name to "FreeTrackCli" (12 characters).

---

### 4. Format Specifier Warnings (Minor)

**Files with warnings (not errors):**
- `src/wine_bridge/client/NPClient_main.c`
- `src/wine_bridge/ft_client/FreeTrackClient_main.c`
- `src/wine_bridge/views/rest.c`

**Problem:** `DWORD` type size differs between 32-bit and 64-bit Wine builds. Using `%d` causes warnings.

**Fix (partial):** Updated format specifiers to use portable casts where applied. Some warnings remain in unmodified files but do not cause compilation failures.

---

### 5. Default Installation Prefix Changed to `/opt/linuxtrack`

**File Modified:**
- `configure.ac`

**Problem:** The default autotools prefix is `/usr/local`, but linuxtrack has always expected `/opt/linuxtrack`. Building with the wrong prefix causes RPATH misalignment where binaries look for libraries in the wrong location, resulting in tracking that silently fails to work.

**Fix:** Added `AC_PREFIX_DEFAULT([/opt/linuxtrack])` to `configure.ac` to set the expected installation prefix as the default.

```m4
# Set default installation prefix to /opt/linuxtrack
# This is the expected location for linuxtrack and ensures RPATH consistency
# Users can still override with --prefix=/custom/path
AC_PREFIX_DEFAULT([/opt/linuxtrack])
```

**Important:** After modifying `configure.ac`, run `autoreconf -fi` to regenerate the configure script.

---

### 6. Relocatable RPATH with `$ORIGIN`

**File Modified:**
- `src/Makefile.am`

**Problem:** RPATH was set to an absolute path (e.g., `/opt/linuxtrack/lib/linuxtrack`), meaning binaries would only work if installed to that exact location. Moving the installation broke library loading.

**Fix:** Changed RPATH to use `$ORIGIN`, which is a special dynamic linker variable that expands to the directory containing the binary.

```makefile
# Line 19 - Before:
LINUXLDLFAGS = '-Wl,-rpath,$(pkglibdir)'

# Line 19 - After:
LINUXLDLFAGS = '-Wl,-rpath,$$ORIGIN/../lib/linuxtrack'

# Line 27 - Before:
FT_LDFLAGS = '-Wl,-rpath,$(libdir)'

# Line 27 - After:
FT_LDFLAGS = '-Wl,-rpath,$$ORIGIN/../lib'
```

**Benefits:**
- Installations are now fully relocatable
- Users can install to any prefix without issues
- Package maintainers have more flexibility

**Important:** After modifying `Makefile.am`, run `autoreconf -fi` to regenerate the build system.

---

### 7. Runtime Path Validation

**File Modified:**
- `src/ltr_server1.c`

**Problem:** When libraries couldn't be found (due to RPATH or configuration issues), tracking would silently fail. Users had no indication of what went wrong.

**Fix:** Added a `validate_paths()` function that checks at startup whether the core library can be found, and prints clear diagnostic messages if not.

```c
static void validate_paths(void) {
  char *lib_path = ltr_int_get_lib_path("libltr");
  
  if (lib_path == NULL) {
    fprintf(stderr, "ERROR: Cannot determine library path!\n");
    // ... detailed diagnostics
    return;
  }
  
  if (access(lib_path, F_OK) != 0) {
    fprintf(stderr, "ERROR: Core library not found!\n");
    fprintf(stderr, "Expected: %s\n", lib_path);
    // ... solutions and debugging tips
  }
  
  free(lib_path);
}
```

**Benefits:**
- Silent failures now produce clear error messages
- Users know exactly what went wrong and how to fix it
- Reduces debugging time significantly

---

## Build Configuration Summary

After running `./configure`, the following features are enabled:

| Feature | Status |
|---------|--------|
| Webcam support | ✅ Yes |
| Wiimote support | ❌ No (libcwiid not installed) |
| TrackIR support | ✅ Yes |
| Facetracker support | ✅ Yes (OpenCV4) |
| XPlane plugin | ✅ Yes |
| Mickey | ✅ Yes |
| Wine plugin | ✅ Yes (32-bit + 64-bit) |
| OSC support | ✅ Yes |

## Dependencies Required

- GCC/G++ (tested with 14.x)
- GNU Autotools (automake, autoconf)
- pkg-config
- Qt 5 or Qt 6 (qmake, Qt5OpenGL, Qt5Help, etc.)
- Mini-XML library (libmxml)
- libusb-1.0
- zlib
- OpenCV 4 (optional, for facetracker)
- libv4l2 (for V4L2 webcam support)
- Wine development files (for wine bridge)
- liblo (for OSC support)
- NSIS (makensis) for Wine installer
- X-Plane SDK headers (optional, for plugin)

## Build Instructions

```bash
cd linuxtrack
./configure
make -j$(nproc)
# Optional: sudo make install
```

## Notes

1. The autotools timestamp issues can be avoided by touching files in the right order if you get `aclocal-1.16` errors.
2. The IDE may show lint errors for Wine bridge files - these are false positives because the IDE doesn't understand the winegcc toolchain's include paths.
3. All changes are minimal and preserve the existing architecture.
