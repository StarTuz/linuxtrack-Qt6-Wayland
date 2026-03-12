# Linuxtrack Modernization - Handoff Document

**Last Updated:** 2026-03-12
**Author:** Antigravity AI Assistant
**Project:** Linuxtrack Head Tracking Software
**Repository:** /home/startux/Code/linuxtrackfixed/linuxtrack

---

## 0. Project Landscape

> **IMPORTANT:** This section provides context about related projects. Read this first.

There are THREE related head tracking projects in this workspace:

### linuxtrackfixed (THIS PROJECT)

**Path:** `/home/startux/Code/linuxtrackfixed/linuxtrack`  
**Philosophy:** Slow, methodical modernization of the original linuxtrack  
**Goal:** Get linuxtrack compiling and working on modern Linux without breaking existing functionality  
**Approach:** Minimal, careful changes. Fix one thing at a time. Document everything.  
**Status:** Active - ✅ Qt6 Migration Complete, TrackIR Game Support Fixed. Follow MODERNIZATION_ROADMAP.md

### tuxtracksold

**Path:** `/home/startux/Code/tuxtracksold/`  
**Philosophy:** First aggressive modernization attempt  
**What Worked:**

- Qt6 compatibility
- Tracking window with camera view and 3D view (modernized for Wayland, backwards compatible with X11)
- X-Plane integration improvements  

**What Failed:** Wine bridge never worked  
**Status:** Archived/Reference only - contains useful code snippets but is not the active project  
**Access Rule:** Only access when explicitly instructed

### tuxtracks (NEW)

**Path:** `/home/startux/Code/tuxtracks/` (when created)  
**Philosophy:** Completely new, modern system built from the ground up  
**Goal:** Modern Linux TrackIR head tracking for today and the future  
**Approach:** Greenfield development, no legacy constraints  
**Status:** New project - will have its own documentation and workflow rules

### Shared Documentation

**Path:** `/home/startux/Code/HEAD_TRACKING_PROTOCOL.md`  
**Purpose:** Protocol specifications that ALL projects must follow for game compatibility

---

## 1. Project Overview

### What is Linuxtrack?

Linuxtrack is a comprehensive head-tracking solution for Linux and Mac that enables users to control applications (primarily games and flight simulators) using head movement. It supports multiple input devices and provides compatibility with Windows games through a Wine bridge.

### Supported Hardware

| Device | Linux Support | Notes |
|--------|---------------|-------|
| TrackIR (1-5) | ✅ Full | USB infrared tracking devices |
| Webcam | ✅ Full | V4L2 webcams with blob tracking |
| Facetracker | ✅ Full | OpenCV-based face tracking |
| Wiimote | ⚠️ Optional | Requires libcwiid |
| PS3 Eye | ✅ Full | Specialized USB camera |

### Key Components

| Component | Type | Description |
|-----------|------|-------------|
| `ltr_gui` | Qt Application | Main configuration GUI |
| `ltr_server1` | Daemon | Background tracking service |
| `liblinuxtrack.so` | Library | Client library for applications |
| `libltr.so` | Library | Core tracking library |
| `libtir.so` | Plugin | TrackIR device driver |
| `libwc.so` | Plugin | Webcam driver |
| `libft.so` | Plugin | Face tracker (OpenCV) |
| `xlinuxtrack9.so` | Plugin | X-Plane 9+ integration |
| `NPClient.dll.so` | Wine DLL | TrackIR emulation for Windows games |
| `FreeTrackClient.dll.so` | Wine DLL | FreeTrack emulation for Windows games |
| `mickey` | Qt Application | Mouse control via head tracking |
| `osc_server` | Server | OSC protocol output for external apps |

---

## 2. Current Session Summary

### Objective

Fix Linuxtrack to compile on modern Linux systems and add native game support.

### Status: ✅ COMPLETE

The project now compiles successfully on modern Linux with:

- GCC 14.x
- Wine 9.x  
- Qt 6.x
- OpenCV 4.x
- CMake (Standard build system)

**Recent Additions (2025-12-26):**

- **Native Games Support:** ltr_udp bridge for X4: Foundations and other OpenTrack-compatible games
- **Global Hotkeys:** ltr_hotkeyd daemon for recenter/pause without alt-tabbing
- **CI Modernization:** Fully automated AppImage build via GitHub Actions, including 32-bit Wine support (multi-pass build).
- **Codebase Cleanup:** Fixed relative includes for robust out-of-tree compilation.  
- **Hotkey GUI:** ltr_hotkey_gui for configuring hotkeys (similar to Controller.exe for Wine)
- **Profile Management:** "New Profile" button in ltr_gui for creating custom profiles
- **Build Cleanup:** Removed stale pre-generated ui_*.h and moc_*.cpp files from source tree
- **Distribution:** Verified working AppImage for Arch Linux systems (built against system Qt6)

**Recent Additions (2026-01-01):**

- **UDP Bridge GUI Integration:** Full UDP stack control from ltr_gui with Start/Stop, hotkey configuration, and Wine/Proton installer

**Recent Additions (2026-01-03) [v1.1.0]:**

- **TrackIR Video On Delay:** Added configurable delay (0-500000µs) after `Video_on` USB command. Essential for hardware revisions where IR LEDs would otherwise flash and turn off immediately. Accessible via TrackIR Troubleshooting GUI.
- **AppImage Stability Fix:** Resolved issue where `Prefix` in configuration would point to volatile AppImage mount points.
- **Stable Library Installation:** `ltr_gui` can now surgically install core libraries to `~/.local/lib/linuxtrack` when installing the X-Plane plugin from an AppImage, ensuring plugin persistence.
- **One Euro Filter:** Implemented an open-source, patent-free adaptive smoothing filter to reduce tracking jitter without adding noticeable lag. Based on the published CHI 2012 algorithm. Toggleable per-axis with two tunable parameters (`min_cutoff` for smoothness, `beta` for responsiveness).
- **Version Bump:** Project version advanced to `1.1.0`.

**Recent Additions (2026-01-04) [v1.1.1]:**

- **One Euro Filter GUI Fix:** Fixed bug where Smoothness/Responsiveness sliders had no effect. The `ltr_srv_slave.c` was incorrectly routing `AXIS_ONE_EURO_ENABLED` and `AXIS_INVERTED` through the float parameter handler instead of the boolean handler. Slider changes now correctly propagate to the tracking server in real-time.
- **One Euro Filter Persistence:** Fixed missing save/load logic for One Euro filter settings. Extended `axis_fields` enum, added save calls in `ltr_int_set_axis_param()`, and added load logic in `ltr_int_get_axis()`. Settings are now saved per-profile (e.g., `Pitch-one-euro-enabled`).
- **X-Plane Seamless Camera Toggle:** Switch between cockpit and external camera views without disabling TrackIR. The plugin now returns early when `view_type != 1026` (3D cockpit), keeping tracking active but not interfering with external views. Added `linuxtrack/ltr_start` and `linuxtrack/ltr_stop` commands for explicit control.
- **X-Plane Recommendation:** Documented that X-Plane users should run `ltr_gui` alongside X-Plane to access One Euro filter controls.
- **Version Bump:** Project version advanced to `1.1.1`.

**Recent Additions (2026-01-04) [v1.1.2]:**

- **ltr_gui --autostart Flag:** Added command-line flag (`--autostart` or `-a`) that automatically starts tracking when the GUI launches. Useful for X-Plane startup scripts.
- **AppImage Qt Plugin Fix:** Fixed "Could not find the Qt platform plugin xcb" error in AppImage. Updated `AppRun` to set `QT_QPA_PLATFORM_PLUGIN_PATH` pointing to bundled platform plugins.
- **Troubleshooting Docs:** Added sections for TrackIR firmware (use Extract method), MFC42 workaround, and NVIDIA 3D view issues.
- **TrackIR Firmware Links:** Updated to point to official [trackir.com/downloads](https://www.trackir.com/downloads/) (v5.53).
- **Version Bump:** Project version advanced to `1.1.2`.

**Recent Additions (2026-01-04) [v1.1.3]:**

- **UDP Bridge Hotkey Daemon Fix:** Fixed bug where clicking "Start UDP Stack" did not spawn the hotkey daemon. The dialog incorrectly tried to start `ltr_wine_hotkeys.exe` via Wine instead of the native `ltr_hotkeyd` daemon. Now correctly starts the native Linux daemon from AppImage, `/opt/linuxtrack/bin`, sibling directory, or PATH.
- **Version Bump:** Project version advanced to `1.1.3`.

**Recent Additions (2026-01-05) [v1.1.4]:**

- **X-Plane Plugin Robustness:** Improved `xlinuxtrack9.c` plugin reliability:
  - **Startup Retry Logic:** Plugin now retries `linuxtrack_init()` every 3 seconds (up to 10 times) if ltr_gui isn't running when X-Plane starts.
  - **Reconnection Logic:** Plugin detects if tracking server dies and attempts to reconnect (up to 3 attempts).
  - **Bounds Checking:** Fixed potential buffer overflow in `messageBox()` with `MAX_MSGBOX_LINES=20` limit.
- **Version Bump:** Project version advanced to `1.1.4`.

**Recent Additions (2026-01-05) [v1.1.5]:**

- **Live Profile Switching:** Profiles can now be changed in ltr_gui while tracking is active. Connected slaves (X-Plane plugin, UDP bridge, etc.) receive the new profile and reinitialize their axes curves immediately.
  - Added `CMD_PROFILE_CHANGE` IPC message
  - Handler in `ltr_srv_slave.c` reinitializes axes
  - Master broadcasts to all affected slaves and re-keys connections
- **Version Bump:** Project version advanced to `1.1.5`.

**Recent Additions (2026-01-05) [v1.1.6]:**

- **Automatic View-Based Tracking Control:** Restored the "automatic pause" behavior from the `fwfa` fork.
  - Tracking now automatically suspends (`linuxtrack_suspend`) when leaving the 3D Cockpit view.
  - Camera coordinates are automatically reset to neutral (`revertView`) in external views to prevent view offsets.
  - Tracking automatically resumes (`linuxtrack_wakeup`) when returning to the cockpit.
- **Version Bump:** Project version advanced to `1.1.6`.

**Recent Additions (2026-01-05) [v1.1.7]:**

- **Fixed View Reset Logic:** Corrected a logic error where `revertView` would fail to reset coordinates when leaving the cockpit. It now supports a forced reset during view transitions.
- **Fixed Shutodwn Freeze:** Resolved a potential deadlock/hang when stopping tracking from the GUI by refining the plugin's reconnection behavior.
- **Version Bump:** Project version advanced to `1.1.7`.

**Recent Additions (2026-01-05) [v1.1.8]:**

- **Fixed TrackIR Freeze:** Added a state guard to ensure `linuxtrack_get_pose` is only called when tracking is in the `RUNNING` state. This prevents X-Plane from freezing if the tracking server is stopped or crashing.
- **Improved Robustness:** Refined reconnection logic to handle the `STOPPED` state gracefully, preventing the plugin from "fighting" the GUI when the user intentionally stops the camera.
- **Reverted View Reset:** Removed the forced coordinate reset on view change, as it was deemed unnecessary. Standard view controls and roll-reset are maintained.
- **Version Bump:** Project version advanced to `1.1.8`.

**Recent Additions (2026-01-05) [v1.1.9]:**

- **Fixed "Frozen View"**: Added an automatic `revertView()` call when the tracking server is stopped or enters an error state. This ensures the cockpit view returns to neutral instead of staying tilted at the last tracked position.
- **Improved Recenter**: The `linuxtrack/ltr_recenter` command now re-bases the X-Plane cockpit view reference point (`pos_init_flag = 1`). This allows users to move their default seat position and recenter tracking relative to the new position.
- **Fixed Coordinate Scaling**: Resolved a regression where head coordinates were being repeatedly scaled even when no new data was available.
- **Fixed Startup Logic**: Ensure the `initialized` flag is correctly set upon successful plugin startup.
- **Version Bump:** Project version advanced to `1.1.9`.

**Recent Additions (2026-01-07) [v1.1.10]:**

- **Video On Delay Merge:** Cherry-picked the Video On Delay feature from main into beta and merged back to main for a stable release.
- **Version Bump:** Project version advanced to `1.1.10`.

**Recent Additions (2026-01-08) [v1.1.11]:**

- **One Euro Filter dt Fix:** Fixed bug where the One Euro filter was not receiving the correct delta time (`dt`) from the tracking loop. The filter now uses the actual frame-to-frame timing instead of a hardcoded value.
- **TrackIR Thread Freeze Fix:** Solved a freeze issue in the TrackIR driver by adding a yield in the read loop.
- **Version Bump:** Project version advanced to `1.1.11`.

**Recent Additions (2026-01-08) [v1.1.12]:**

- **X-Plane Plugin Installation Fix:** Removed reference to non-existent `xlinuxtrack9_32.so` (32-bit plugin that was never built). The installer now correctly installs only the 64-bit plugin (`xlinuxtrack9.so`) to the `64/` directory, matching modern X-Plane 11/12 plugin structure.
- **AppImage Qt Plugin Fix:** When manually building AppImages (bypassing linuxdeploy), Qt6 platform plugins must be explicitly bundled. See Section 3.16 for details.
- **Version Bump:** Project version advanced to `1.1.12`.

**Recent Additions (2026-03-10) [v1.3.5 — Webcam tracking fixes]:**

- **Webcam Format Correctness (UI):** Fixed three bugs in `src/qt_gui/webcam_info.cpp`:
  1. `GREY` was missing from `isNativeFormat()`, causing the UI to display "Driver-dependent" instead of "Native" for greyscale webcams.
  2. The `kYUYV` sentinel constant was initialised via `*"YUYV"` (pointer dereference = single char `'Y'`). Replaced with a proper four-byte literal so the default fallback format is correctly identified.
  3. `decodeRes` / `decodeFps` regex patterns were too strict; updated to accept formats used by real webcam drivers.

- **Webcam Setup UI Improvements:** Added webcam preview mode toggle, webcam negotiation diagnostics panel, labeled webcam mode support, and disambiguated multiple webcams of the same model via device identifier (path/serial).

- **Regression Test Infrastructure:** Added Catch2 v3 (amalgamated) test suite `src/tests/test_webcam_info.cpp` covering the three bugs above (12 test cases, 34 assertions). Tests are wired into CTest (`ctest --output-on-failure`) and run automatically in CI on both Ubuntu and Arch Linux. Added `stubs_dyn_load.cpp` so the test binary links without requiring a live webcam library.

- **Webcam Tracking Regression — MJPG passthrough (critical):** When the user configured `Pixel-format=MJPG`, `read_pref_format` in `src/webcam_driver.c` passed MJPG directly to `VIDIOC_S_FMT`. libv4l2 only decodes MJPG→YUV transparently when the *application* requests a YUV format; if the app explicitly requests MJPG, libv4l2 forwards raw JPEG bytes unchanged. `get_gray_image` has no JPEG decoder, so every frame produced a black image → no blobs → tracking appeared dead. **Fix:** Redirect MJPG/JPEG to YUYV in the `v4l2_format` struct before format negotiation. `wc_info.requested_fourcc` retains the original value for diagnostic logs.

- **Webcam Tracking Regression — blob size limits (critical):** `min_blob=4, max_blob=230` were calibrated at 160×120. At 800×600 (~25× the reference area) the same physical IR LED covers ~250–1250 pixels, so every blob was rejected by `max_blob=230`. **Final Fix:** Keep the shared blob extractor (`src/image_process.c`) device-agnostic and apply the 160×120 area scaling only in webcam-style callers (`src/webcam_driver.c`, `src/ps3eye_driver.c`). This preserves the intended webcam fix without changing TrackIR blob semantics.

- **TrackIR Regression From Webcam Blob Scaling (critical, 2026-03-11):** The first webcam fix implementation scaled blob-size thresholds globally inside `ltr_int_stripes_to_blobs` (`src/image_process.c`). TrackIR uses the same extractor via `src/tir_img.c`, but its `Min-blob` / `Max-blob` values are already calibrated for native TrackIR resolutions (for example, 640×480 on TrackIR 5). Applying the webcam scale factor to TrackIR inflated `120/2500` to `1920/40000`, which poisoned the TrackIR 3-point pose path: bright points were visible in the camera view, but the 3D view produced no usable pose. **Fix:** Move the scaling out of the shared extractor and make it opt-in for webcam / PS3Eye callers only.

**Recent Additions (2026-03-11) [v1.3.7 — TrackIR regression coverage and webcam face tuning]:**

- **Version Bump:** Project version advanced to `1.3.7`.

- **TrackIR Regression Test Coverage:** Added `src/tests/test_image_process.cpp` coverage that locks down the shared blob extractor semantics. The test explicitly guards the failure mode where webcam-oriented blob scaling at 640×480 would have filtered out TrackIR-sized blobs before the 3-point pose solver ever ran.

- **Webcam Face Tracking: Camera FOV Control:** Added a `Camera-fov` preference for the webcam face tracker (`src/wc_driver_prefs.c`) and exposed it in the Linux and macOS webcam-face setup dialogs. The neural tracker now uses the configured FOV instead of a hardcoded wide angle, which makes forward/back movement tunable per camera.

- **Webcam Face Tracking: Pose Stability:** The neural face path now keeps the last valid pose across brief detector misses, clamps bad `dt` values after stalls, and applies stronger translation/angle filtering with a short median stage. This reduced visible jitter without touching the TrackIR path.

- **Webcam Face Tracking: Translation Scale:** Removed the accidental `0.1x` translation downscale in `src/neuralnet_tracker.cpp`. The shared pose layer expects translations in millimeters, so the old scale reduction made webcam forward/back movement far too subtle compared to TrackIR.

- **Webcam Face Tracking: Forward Translation Gain:** Added a Z-only gain so webcam face tracking has a stronger forward/back feel without changing X/Y motion semantics.

- **Qt GUI Slider Audit:** Tuned the common smoothing mapping, corrected the One Euro `Smoothness` slider direction so moving right actually means smoother, fixed detailed-axis deadzone/sensitivity refresh bugs, and reworked the webcam face smoothing slider curve so the useful range is not pushed to the far right.

- **Release Pipeline Follow-up Fixes (2026-03-12):**
  - Fixed a release-only facetracker regression where `src/facetrack.cpp` referenced `use_neuralnet` outside `#ifdef HAVE_ONNXRUNTIME`, breaking non-ONNX builds in CI.
  - Repaired the AppImage packaging scripts so they resolve the installed Linuxtrack icon from the hicolor tree first and only fall back to `share/pixmaps` if needed.
  - Restored X-Plane plugin support in tagged releases by adding the X-Plane SDK header installation step to `.github/workflows/release.yml`, matching the normal CI build path.
  - Removed the obsolete Catch2 download steps from CI/release workflows because the amalgamated Catch2 sources are already vendored in `src/tests/catch2/`.
  - Updated workflow actions to current Node 24-compatible lines and opted workflows into Node 24 execution to address GitHub's Node 20 deprecation warnings.

- **macOS Bring-up Planning (2026-03-11):** Added [doc/MACOS_ROADMAP.md](/home/startux/Code/linuxtrackfixed/linuxtrack/doc/MACOS_ROADMAP.md), a phased implementation plan for future macOS work. The plan keeps macOS behind an experimental build gate, requires shared-core regression coverage before platform-specific changes, and explicitly stages the work as: app shell, webcam/face tracking, X-Plane plugin support, then TrackIR hardware support.
- **macOS Bring-up Phase 0 (2026-03-11):** Implemented the initial containment gate for future macOS work. Added `BUILD_MAC_EXPERIMENTAL` to [CMakeLists.txt](/home/startux/Code/linuxtrackfixed/linuxtrack/CMakeLists.txt), moved historical mac-specific Qt GUI sources/forms behind `APPLE AND BUILD_MAC_EXPERIMENTAL` in [src/qt_gui/CMakeLists.txt](/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/CMakeLists.txt), and switched [device_setup.cpp](/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/device_setup.cpp) to compile mac-specific preference panels only when `LTR_MAC_EXPERIMENTAL` is defined. Verified with a fresh Linux configure, `ltr_gui` build, and the existing four CTest targets.
- **macOS Bring-up Phase 1 (2026-03-11, in progress):** Added shared-core regression coverage in [src/tests/test_pref_global.cpp](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tests/test_pref_global.cpp) and [src/tests/test_tracking.cpp](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tests/test_tracking.cpp), registered in [src/tests/CMakeLists.txt](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tests/CMakeLists.txt). The new tests lock down mac-related device-category mapping, single-point face `tz` behavior, face routing to the absolute pose path, and 3-point solver failure semantics. This work also uncovered and fixed a real bug in [src/tracking.c](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tracking.c): `ltr_int_update_pose()` was using a `bool` local for its return status, which turned `-1` failure/discard paths into `1`.
- **macOS Bring-up Phase 1 (2026-03-11, continued):** Extended [src/tests/test_tracking.cpp](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tests/test_tracking.cpp) to cover `ltr_int_postprocess_axes()`. The new checks lock down translation passthrough when alignment is disabled, rotation of translations when alignment is enabled, and rejection of non-finite raw axis values before filtering. Verified again with the full six-test CTest suite in `build_phase0_verify`.
- **macOS Bring-up Phase 1 (2026-03-11, continued):** Extended [src/tests/test_tracking.cpp](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tests/test_tracking.cpp) again to lock down camera-orientation normalization before the 3-point solver sees blobs, plus `ORIENT_FROM_BEHIND` sign handling on solved 3-point pose values. Verified again with the full six-test CTest suite in `build_phase0_verify`.
- **macOS Bring-up Phase 1 (2026-03-11, continued):** Extracted the camera-orientation normalization logic into shared helpers in [src/tracking.h](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tracking.h) and [src/tracking.c](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tracking.c): `ltr_int_rotate_camera_point()` and `ltr_int_normalize_bloblist_for_camera_orientation()`. [src/tests/test_tracking.cpp](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tests/test_tracking.cpp) now covers those helpers directly, so future macOS frame-source work can reuse the exact same semantics instead of depending on a private `tracking.c` implementation detail.
- **macOS Bring-up Phase 1 (2026-03-11, continued):** Extracted pose-route selection into [src/tracking.h](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tracking.h) and [src/tracking.c](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tracking.c) as `ltr_int_select_pose_route()`. [src/tests/test_tracking.cpp](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tests/test_tracking.cpp) now verifies the shared routing semantics directly, so future platform work does not have to rediscover the implicit “face with 3 blobs => absolute, single-point => 1pt, otherwise 3pt” decision tree hidden inside `ltr_int_update_pose()`.
- **macOS Bring-up Phase 1 (2026-03-12, completed):** Extracted frame snapshot copying into [src/tracking.h](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tracking.h) and [src/tracking.c](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tracking.c) as `ltr_int_snapshot_frame_to_pose()`. [src/tests/test_tracking.cpp](/home/startux/Code/linuxtrackfixed/linuxtrack/src/tests/test_tracking.cpp) now locks down resolution, blob coordinate, and score copying directly, giving future macOS frame-source work an explicit shared seam for moving captured frames into `linuxtrack_full_pose_t` before platform-specific capture code exists. Rebuilt `test_tracking` and reran the full six-test CTest suite in `build_phase0_verify`, completing the shared-core hardening exit criteria for Phase 1.

---

## 3. Changes Made

### 3.1 Missing pthread.h Includes

**Problem:** Modern GCC has stricter header isolation. `pthread.h` is no longer implicitly included through other headers.

**Files Modified:**

1. **`src/ltr_srv_master.cpp`** (Line 3)

   ```cpp
   #include <pthread.h>
   ```

2. **`src/pref.cpp`** (Line 5)

   ```cpp
   #include <pthread.h>
   ```

**Error Fixed:**

```
error: 'PTHREAD_MUTEX_INITIALIZER' was not declared in this scope
```

---

### 3.2 Wine DLL_WINE_PREATTACH Removal

**Problem:** `DLL_WINE_PREATTACH` was an internal Wine extension removed in Wine 6.0 (February 2021).

**Files Modified:**

1. **`src/wine_bridge/client/NPClient_main.c`** (Lines 55-58)

   ```c
   #ifdef DLL_WINE_PREATTACH
       case DLL_WINE_PREATTACH:
           return TRUE;
   #endif
   ```

2. **`src/wine_bridge/ft_client/FreeTrackClient_main.c`** (Lines 63-67)

   ```c
   #ifdef DLL_WINE_PREATTACH
   case DLL_WINE_PREATTACH:
       return TRUE;
   #endif
   ```

**Error Fixed:**

```
error: 'DLL_WINE_PREATTACH' undeclared (first use in this function)
```

**Note:** Using `#ifdef` maintains backward compatibility with older Wine versions.

---

### 3.3 Wine Debug Channel Name Length

**Problem:** Wine debug channel names have a 15-character limit (14 + NUL). "FreeTrackClient" is exactly 15 characters.

**File Modified:**

**`src/wine_bridge/ft_client/FreeTrackClient_main.c`** (Line 22)

```c
// Before:
WINE_DEFAULT_DEBUG_CHANNEL(FreeTrackClient);

// After:
WINE_DEFAULT_DEBUG_CHANNEL(FreeTrackCli);
```

**Error Fixed:**

```
error: static assertion failed: "sizeof(\"FreeTrackClient\") <= sizeof(__wine_dbch_FreeTrackClient.name)"
```

---

### 3.4 Format Specifier Fix (Partial)

**Problem:** `DWORD` type is `unsigned int` on 32-bit Wine and `unsigned long` on 64-bit Wine.

**Files Partially Fixed:**

- `NPClient_main.c`: Changed `%d` to `%ld` for DWORD
- `FreeTrackClient_main.c`: Changed to `%lu` with cast

**Remaining Warnings (Non-blocking):**

- `src/wine_bridge/views/rest.c:219` - `%d` for `GetLastError()`

---

### 3.5 Qt6 Migration & 3D View Modernization

**Status:** ✅ COMPLETE

**Problem:** `QGLWidget` and fixed-function OpenGL (`glBegin`, `glCallList`) are deprecated in Qt6 and do not work on Wayland/EGL.

**Fixes:**

1. **Dual Support**: Updated headers and `.pro` files to support both Qt5 and Qt6.
2. **Modern OpenGL**: Completely refactored `GLWidget` for Qt6 to use `QOpenGLWidget`, `QOpenGLVertexArrayObject`, `QOpenGLBuffer`, and `QOpenGLShaderProgram`.
3. **Shaders**: Implemented GLSL ES 3.0 shaders for vertex and fragment processing.
4. **Transformation Logic**: Migrated from OpenGL matrix stack to `QMatrix4x4` for MVP and normal matrices.
5. **Geometry**: Converted on-the-fly OBJ parsing into static VBO/VAO buffers for performance and Wayland compatibility.

### 3.6 Wine Bridge & Path Fixes

**Status:** ✅ COMPLETE

**Problem:** Typos in `Makefile.am` and missing RPATH logic in the Wine bridge bridge caused games (like Elite Dangerous) to fail to find tracking libraries after the rebuild.

**Fixes:**

1. **Global Fix**: Fixed typos in `src/Makefile.am` (`LINUXLDLFAGS` -> `LINUXLDFLAGS`).
2. **Bridge Fix**: Added `-Wl,-rpath` flags to `src/wine_bridge/client/Makefile.am` and `src/wine_bridge/ft_client/Makefile.am`.
3. **Relocatable Binaries**: Used `$ORIGIN` to ensure binaries can find libraries in both build and install (`/opt/linuxtrack/lib/linuxtrack`) locations.

### 3.7 Segmentation Fault Fixes

**Status:** ✅ COMPLETE

**Problem:** `ltr_gui` crashed on exit during cleanup.

**Fixes:**

1. **ProfileSelector**: Added null check for `ProfileSetup` pointer during close event.
2. **GLWidget Destructor**: Implemented safe, unique-pointer deletion for textures to avoid double-free errors.
3. **Context Management**: Ensured `makeCurrent()` / `doneCurrent()` calls wrap GL resource deletion.

---

### 3.8 Preference System Modernization (Bison Removal)

**Status:** ✅ COMPLETE

**Problem:** The legacy Bison/Flex parser for `.conf` files was brittle, hard to maintain, and prone to build issues with modern tools.

**Fixes:**

1. **Modern Backend**: Migrated to the `mINI` header-only library for robust INI file parsing.
2. **Bridge Layer**: Implemented a bridge in `pref.cpp` that maintains compatibility with the existing C API (`ltr_int_get_key`, etc.) and the legacy C++ `prefs` class used by the GUI.
3. **Cleanup**: Removed `pref_bison.ypp` and `pref_flex.lpp` from the build system, simplifying project dependencies.

### 3.9 GUI Polish & Qt6 Fixes

**Status:** ✅ COMPLETE

**Problem:** Deprecated Qt5 signals or broken auto-connections in Qt6.

**Fixes:**

1. **Model Setup**: Fixed the "Model Setup" images not updating by manually connecting `QComboBox::textActivated` to a new slot, bypassing the removed `activated(QString)` signal.

### 3.10 Wine Support Modernization (Proton Compatibility)

**Status:** ✅ COMPLETE

**Problem:** The legacy Wine bridge installation relied on a fragile NSIS installer, downloading 20-year-old Microsoft runtimes (VC6), and using "Fake Wine" environments to extract firmware, which is incompatible with modern Proton/Steam Deck environments.

**Fixes:**

1. **Surgical Injection**: Implemented a native "Surgical Injection" routine in `PluginInstall::installLinuxtrackWine`. It now directly copies compiled bridge files (`NPClient.dll`, `FreeTrackClient.dll`, `Controller.exe`) to the prefix and updates the registry via native `wine reg` calls.
2. **Controller.exe for Hotkeys**: Now includes `Controller.exe` in surgical injection for customizable Pause/Recenter hotkeys. Uses DirectInput for global keyboard capture that works even when games have focus.
3. **Dual Program Files Installation**: Installs to both `C:\Program Files\Linuxtrack\` and `C:\Program Files (x86)\Linuxtrack\` for 32-bit and 64-bit game compatibility.
4. **Proton Tuning**: `WineLauncher` now automatically sets `PROTON_NO_FSYNC=1` and `PROTON_NO_ESYNC=1` to prevent race conditions in tracking data.
5. **Automatic Prefix Discovery**:
    - ✅ **Qt6 entryList Fix**: Fixed Steam game discovery by switching from Qt's glob patterns (broken in Qt6 with C locale) to `entryInfoList` with manual filtering.
    - ✅ **Steam/Proton**: Scans all Steam library folders including custom mount points.
    - ✅ **Lutris**: Parses game YAML configs for Wine prefixes.
    - ✅ **Bottles**: Detects Flatpak and native Bottles installations.
    - ✅ **X-Plane 12**: Auto-detection for native X-Plane installation paths.
6. **Native Harvesting**: Refactored firmware extraction to prioritize native tools (`7z`). Can unpack TrackIR installers without Wine.
7. **Verification**:
    - ✅ **DCS World**: Confirmed successful with Controller.exe hotkeys (Proton).
    - ✅ **Elite Dangerous**: Confirmed successful on **Proton 10**.
    - ✅ **X-Plane 12**: Native plugin confirmed working.
    - ✅ **Fresh Prefixes**: Confirmed tracking works on fresh 64-bit Wine prefixes (Registry fix).

### 3.11 Deprecated API Replacement & Code Safety

**Status:** ✅ COMPLETE

**Problem:** Several deprecated C APIs (`sprintf`, `strtok`) posed buffer overflow and thread-safety risks.

**Fixes:**

1. **Wine Bridge (High Priority)**:
    - `wine_bridge/client/check_data.c`: `sprintf` → `snprintf` with tracked buffer size
    - `wine_bridge/client/rest.c`: All path construction now uses `snprintf`
    - `wine_bridge/ft_tester/main.cpp`: `sprintf` → `snprintf` for DLL path
2. **Core Library**:
    - `webcam_driver.c`: `snprintf` for debug frame filenames
    - `image_process.c`: `snprintf` for debug data filenames
    - `linuxtrack.c`: `strtok` → `strtok_r` for thread-safe `LINUXTRACK_LIBS` parsing

### 3.13 GLWidget Race Condition Fix

**Status:** ✅ COMPLETE

**Problem:** 3D View displayed black due to race condition between `ReaderThread` (loading model objects) and `initializeGL()` (building OpenGL buffers).

**Fix:**

1. Added `objectsLoaded` and `glInitialized` flags to `GLWidget`.
2. `initializeGL()` sets `glInitialized = true` and calls `makeObjects()` if `objectsLoaded` is already true.
3. `objectsRead()` sets `objectsLoaded = true` and calls `makeObjects()` if `glInitialized` is already true.

### 3.14 Fresh Prefix & Proton Fix (Critical)

**Status:** ✅ COMPLETE

**Problem:** Fresh 64-bit Wine/Proton prefixes (like a new Elite Dangerous install) failed to track. Surgical injection appeared successful, but games couldn't find `NPClient.dll`.

**Root Cause:**
The `WineLauncher` helper class was forcing `WINEARCH=win32` environment variable for all internal commands. When running `reg.exe` to inject registry keys into a 64-bit prefix, this mismatch caused the registry write to fail or go to the wrong hive, leaving the game without the necessary `HKCU\Software\NaturalPoint` keys.

**Fix:**

- Removed `WINEARCH=win32` from `WineLauncher.cpp`. The launcher now respects the prefix's native architecture.
- Added `PROTON_NO_FSYNC=1` and `PROTON_NO_ESYNC=1` to the `WineLauncher` environment to protect internal tools, but **removed** the requirement for users to set this manually.
- **Verification:** Validated that tracking works out-of-the-box on fresh prefixes without manual Steam Launch Options.

### 3.15 Project LAL (Licensed Asset Loader)

**Status:** ✅ COMPLETE (Prototype)

**Problem:** The legacy method of extracting proprietary firmware (TIRViews.dll) relied on a fragile Wine-based "fake environment" that was hard to maintain and security-prone.

**Fix:**

1. **LALManager**: Implemented a new C++ backend that natively extracts assets using `7z` or `tar`, with no Wine dependency.
2. **Manifest-Driven**: Assets are defined in `lal_manifest.json`, making updates easy without recompiling.
3. **LALDialog**: Added a native Qt GUI dialog ("Manage Assets (LAL)...") to the Misc tab, allowing users to browse and install firmware archives easily.
4. **Verification**: Verified UI functionality and native extraction logic via unit tests.

### 3.18 Webcam Tracking Regression Fixes (2026-03-10)

**Status:** ✅ COMPLETE

#### Root Cause 1 — MJPG passthrough (`src/webcam_driver.c`)

**Problem:** `read_pref_format` set `fmt->fmt.pix.pixelformat` to whatever the user configured. When the user selected `MJPG`, libv4l2 passed raw JPEG byte-streams to the application unchanged. `get_gray_image` has no JPEG decoder; the result was a black frame every capture cycle → no blobs detected → tracking appeared completely dead.

**Fix:** After recording `wc_info.requested_fourcc` (for diagnostics), redirect any MJPG/JPEG pixelformat in the `v4l2_format` struct to YUYV before passing it to `VIDIOC_S_FMT`. libv4l2 then performs the MJPG→YUYV decode transparently:

```c
static const __u32 kMJPG = V4L2_PIX_FMT_MJPEG;
static const __u32 kJPEG = V4L2_PIX_FMT_JPEG;
static const __u32 kYUYV = V4L2_PIX_FMT_YUYV;
if (fmt->fmt.pix.pixelformat == kMJPG || fmt->fmt.pix.pixelformat == kJPEG) {
    ltr_int_log_message("Compressed pixel format requested; redirecting to YUYV for libv4l2 decode.\n");
    fmt->fmt.pix.pixelformat = kYUYV;
}
```

#### Root Cause 2 — Absolute blob size limits (`src/image_process.c`)

**Problem:** `min_blob` and `max_blob` preferences (defaults 4 and 230) are raw pixel-count thresholds calibrated for 160×120 — the original hardcoded fallback resolution. At 800×600 (25× more pixels), the same IR LED subtends ~250–1250 pixels, so every candidate blob was rejected by `max_blob=230`.

**Initial Fix:** Scale both limits proportionally to the current image area before filtering:

```c
float area_scale = ((float)img->w * (float)img->h) / (160.0f * 120.0f);
int scaled_min = (int)(min_pts * area_scale);
int scaled_max = (int)(max_pts * area_scale);
if ((pb->points < scaled_min) || (pb->points > scaled_max)) {
    continue;
}
```

This makes webcam thresholds resolution-invariant, but the first implementation applied it too low in the stack.

**Final Fix (2026-03-11):** Keep `ltr_int_stripes_to_blobs` device-agnostic and move the scaling to webcam-style callers only:

```c
int min_blob_pixels = wc_info.min_blob_pixels;
int max_blob_pixels = wc_info.max_blob_pixels;
ltr_int_scale_blob_limits_for_resolution(&img, &min_blob_pixels,
                                         &max_blob_pixels);
ltr_int_stripes_to_blobs(MAX_BLOBS, &(f->bloblist), min_blob_pixels,
                         max_blob_pixels, &img);
```

TrackIR still calls `ltr_int_stripes_to_blobs(..., min, max, img)` directly from `src/tir_img.c`, so its device-specific thresholds are no longer distorted by a webcam-specific assumption.

---

### 3.19 Regression Test Infrastructure — Webcam (2026-03-10)

**Status:** ✅ COMPLETE

Added `src/tests/test_webcam_info.cpp` (Catch2 v3 amalgamated) — 12 test cases / 34 assertions covering:

| Test group | What it guards |
|---|---|
| `describeFormatPolicy` — native formats | YUYV, YU12, YV12, RGB3, BGR3, GREY all → "Native" |
| `describeFormatPolicy` — GREY regression | GREY must not produce "Driver-dependent" |
| `describeFormatPolicy` — compressed | MJPG, JPEG → "Compressed" |
| `describeFormatPolicy` — unknown | NV12 → "Driver-dependent" |
| `describeFormatPolicy` — single-char Y | "Y" must not be classified Native (guards kYUYV pointer-deref bug) |
| `decodeRes` | Standard, large, uppercase-X formats; garbage rejection |
| `decodeFps` | Rational `num/den` with/without spaces; garbage rejection |

`src/tests/stubs_dyn_load.cpp` stubs `ltr_int_load_library` → nullptr so the test binary links without `libwc.so`. Both tests (`test_lal`, `test_webcam_info`) run via `ctest` in CI on Ubuntu 24.04 and Arch Linux.

---

### 3.20 TrackIR Regression From Webcam Blob Scaling (2026-03-11)

**Status:** ✅ COMPLETE

**Symptom:** After the webcam blob-size fix landed, TrackIR still showed bright points / blobs in the camera view, but the 3D view stopped moving. This looked like a TrackIR pose regression even though the original change targeted webcams.

**Root Cause:** The first webcam fix version placed the 160×120 area scaling inside the shared blob extractor `ltr_int_stripes_to_blobs` (`src/image_process.c`). That function is also used by TrackIR through `src/tir_img.c`. TrackIR blob thresholds are already device-specific (`Min-blob`, `Max-blob` in the TrackIR section), so the extra scaling changed their meaning. On a 640×480 TrackIR 5 stream, the scale factor is `16x`, turning `120/2500` into `1920/40000` before the 3-point pose solver ever sees the blob list.

**Why This Broke TrackIR But Not Webcam:** Webcam thresholds were historically tuned around 160×120 preview frames, so they benefit from resolution compensation. TrackIR thresholds were already tuned at native device resolution. A shared fix was the wrong abstraction boundary.

**Fix:** Revert `src/image_process.c` to use the raw thresholds passed in by the caller, add helper `ltr_int_scale_blob_limits_for_resolution(...)`, and call it only from webcam-style drivers:

- `src/webcam_driver.c`
- `src/ps3eye_driver.c`

TrackIR (`src/tir_img.c`) now keeps its original thresholds unchanged.

**Verification:**

1. Rebuilt `libltr.so` and `ltr_gui`.
2. Ran `ctest --output-on-failure` successfully (`test_lal`, `test_webcam_info`).
3. Hardware symptom resolved: TrackIR camera view still shows blobs and the 3D view moves again.

---

### 3.17 X-Plane Plugin Missing from AppImage (v1.3.5+)

**Status:** ✅ COMPLETE

**Problem:** `xlinuxtrack9.so` was never included in CI-produced AppImages. The CMakeLists.txt conditionally builds the plugin only when X-Plane SDK headers exist at `/usr/include/xplane_sdk/XPLM`. The CI build environment (Ubuntu 24.04) has no apt package for this SDK, so CMake silently skipped it every time.

**Fix:**

Added an "Install X-Plane SDK headers" step to `.github/workflows/build.yml` before the main build pass. Downloads XPSDK 4.3.0 from the official Laminar Research URL, finds `XPLMPlugin.h` by filename (robust against zip structure changes), and installs headers to `/usr/include/xplane_sdk/XPLM/` and `/usr/include/xplane_sdk/Widgets/`.

**Secondary Issue:** Once the plugin was built, linuxdeploy's `--library` loop in `make_appimage.sh` copied it to `usr/lib/` in addition to the correct `usr/lib/linuxtrack/` location. `xlinuxtrack9.so` is an X-Plane plugin, not a runtime library, so the `usr/lib/` copy was spurious. Fixed by excluding `xlinuxtrack9*.so` from the `--library` loop.

**Correct AppImage layout:**
- `usr/lib/linuxtrack/xlinuxtrack9.so` ✅ (only location; used by ltr_gui's X-Plane installer)
- `usr/lib/xlinuxtrack9.so` ✗ (was a linuxdeploy artifact — now removed)

---

### 3.16 AppImage Packaging

**Status:** ✅ COMPLETE

**Problem:** Running `ltr_gui` from the build directory caused permissions/loading issues, and the traditional `sudo make install` approach is difficult to distribute to end users.

**Fix:**

1. **Workflow**: Created `.github/workflows/build.yml` to automatically build an AppImage on every push.
2. **Script**: Added `packaging/appimage/make_appimage.sh` which uses `linuxdeploy` and `linuxdeploy-plugin-qt`.
3. **Result**: Users can now download a single, portable `Linuxtrack-x86_64.AppImage` executable that works on most modern Linux distributions (Ubuntu 22.04+, Fedora, Arch, etc.).

**Manual AppImage Build Notes (v1.1.12+):**

If `linuxdeploy` fails with strip errors on newer systems (`.relr.dyn` section errors), you can build manually with `appimagetool`. However, Qt plugins must be explicitly bundled:

```bash
# 1. Install to AppDir
DESTDIR=$(pwd)/AppDir cmake --install build --prefix /usr

# 2. Copy Qt6 platform plugins (CRITICAL)
mkdir -p AppDir/usr/plugins/platforms
cp /usr/lib/qt6/plugins/platforms/libqxcb.so AppDir/usr/plugins/platforms/
cp -r /usr/lib/qt6/plugins/xcbglintegrations AppDir/usr/plugins/

# 3. Setup AppImage resources
cp AppDir/usr/share/applications/linuxtrack.desktop AppDir/
cp AppDir/usr/share/pixmaps/linuxtrack.svg AppDir/
ln -sf linuxtrack.svg AppDir/.DirIcon
cp packaging/appimage/AppRun AppDir/ && chmod +x AppDir/AppRun

# 4. Build AppImage
ARCH=x86_64 appimagetool AppDir Linuxtrack-x86_64.AppImage
```

**Required Qt6 Plugins:**

| Plugin Path | Purpose |
|-------------|--------|
| `usr/plugins/platforms/libqxcb.so` | X11/XCB display support (required) |
| `usr/plugins/xcbglintegrations/libqxcb-egl-integration.so` | EGL OpenGL integration |
| `usr/plugins/xcbglintegrations/libqxcb-glx-integration.so` | GLX OpenGL integration |

---

## 4. Build Configuration

### Configure Output Summary

```
Webcam support........... yes
Wiimote support.......... no (libcwiid not found)
TrackIR support.......... yes
Facetracker support...... yes (OpenCV4)
XPlane plugin............ yes
Mickey................... yes
Wine plugin.............. yes (32-bit + 64-bit)
OSC support.............. yes
```

### Build Commands (Modern)

```bash
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)
# Optional: sudo cmake --install .
```

### Required Dependencies (Arch Linux)

```bash
# Core build tools
base-devel cmake ninja pkg-config

# Libraries
mxml libusb zlib

# Qt (for GUI)
qt6-base qt6-tools qt6-5compat qt6-help

# Video/Webcam
v4l-utils

# OpenCV (for facetracker)
opencv

# Wine (for Windows game bridge)
wine wine-mono wine-gecko

# Optional dependencies
liblo                    # OSC support
xplane-sdk               # X-Plane plugin
libcwiid                 # Wiimote support
```

---

## 5. Build Artifacts

### Libraries (in `src/.libs/`)

| File | Description |
|------|-------------|
| `liblinuxtrack.so` | Main client library |
| `liblinuxtrack32.so` | 32-bit client library |
| `libltr.so` | Core tracking library |
| `libtir.so` | TrackIR driver |
| `libltusb1.so` | libusb-1.0 interface |
| `libwc.so` | Webcam driver |
| `libft.so` | Face tracker driver |
| `libjoy.so` | Joystick driver |
| `xlinuxtrack9.so` | X-Plane 9+ plugin (64-bit only) |

### Executables (in `src/`)

| File | Description |
|------|-------------|
| `ltr_server1` | Main tracking server daemon |
| `ltr_gui` | Qt configuration GUI |
| `ltr_pipe` | Pipe-based tracking interface |
| `ltr_extractor` | Game data extractor |
| `ltr_recenter` | Recenter command utility |
| `osc_server` | OSC protocol server |
| `mickey` | Mouse control application |
| `ltr_udp` | X4: Foundations / native games (OpenTrack UDP) bridge |
| `ltr_hotkeyd` | Global hotkey daemon for native games (X11) |
| `ltr_hotkey_gui` | Qt GUI for configuring hotkeys |

### Wine Bridge (in `src/wine_bridge/`)

| File | Description |
|------|-------------|
| `client/NPClient.dll.so` | 32-bit TrackIR emulation DLL |
| `client/NPClient64.dll.so` | 64-bit TrackIR emulation DLL |
| `ft_client/FreeTrackClient.dll.so` | FreeTrack emulation DLL |
| `linuxtrack-wine.exe` | NSIS installer for Wine prefix |
| `controller/Controller.exe` | DirectInput controller |
| `tester/Tester.exe` | 32-bit testing utility |
| `tester/Tester64.exe` | 64-bit testing utility |
| `views/TrackIR.exe` | TrackIR status viewer |

---

## 6. Architecture Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                        Applications                              │
├─────────────┬─────────────┬─────────────┬──────────────────────┤
│  X-Plane    │   Native    │   OSC       │   Windows Games      │
│  Plugin     │   Linux     │   Clients   │   (via Wine)         │
│xlinuxtrack9 │   Apps      │             │                      │
├─────────────┴─────────────┴─────────────┴──────────────────────┤
│                     liblinuxtrack.so                            │
│                  (Client Library API)                           │
├─────────────────────────────────────────────────────────────────┤
│                      ltr_server1                                │
│                 (Background Daemon)                             │
├─────────────────────────────────────────────────────────────────┤
│                       libltr.so                                 │
│                 (Core Tracking Engine)                          │
├─────────────┬─────────────┬─────────────┬──────────────────────┤
│   libtir    │   libwc     │   libft     │   libp3e            │
│  (TrackIR)  │  (Webcam)   │ (Facetrack) │  (PS3 Eye)          │
├─────────────┴─────────────┴─────────────┴──────────────────────┤
│                     Hardware Devices                            │
│          TrackIR USB | Webcam V4L2 | PS3 Eye USB               │
└─────────────────────────────────────────────────────────────────┘
```

---

## 7. Known Issues & Limitations

### Current Issues

1. **Relocatable Installation** ✅ FIXED
   - CMake build now uses `$ORIGIN`-based RPATHs.
   - Binaries and libraries are relocatable and find each other automatically.
   - No longer dependent on fixed absolute paths in the binaries.

2. **Wiimote Support Disabled**
   - Requires `libcwiid` package
   - Install: `yay -S libcwiid` (AUR)

3. **Format Specifier Warnings**
   - `rest.c:219` has `%d` for `DWORD`
   - Non-blocking, cosmetic only

4. **IDE Lint Errors**
   - Wine bridge files show false-positive errors in IDEs
   - Cause: IDE doesn't understand `winegcc` include paths
   - These files compile correctly with the Wine toolchain

5. **X-Plane External View Conflict** ⚠️ UNDER INVESTIGATION
   - **Symptom:** Cannot switch to external view (Shift-4) while TrackIR is active
   - **Visual:** Horizon lights/textures flash rapidly, as if view is being pulled back to cockpit
   - **Workaround:** Pause tracking before switching views, or use `linuxtrack/ltr_pause` command
   - **Root Cause:** Plugin applies cockpit coordinates every frame, overriding X-Plane's view change
   - **Investigation Notes:** See `.agent/TEAM_FEEDBACK.md` post-mortem section
   - **Reference:** fwfa123/linuxtrackx-ir fork reportedly handles this correctly
   - **Status:** Requires proper side-by-side diff with fwfa fork to isolate fix
   - **Next Steps (Team Recommendations):**
     1. Check if `pv_present` is incorrectly true (bypasses view_changed guard)
     2. Add debug logging with `XPLMDebugString()` to trace view_type during Shift-4
     3. Line-by-line comparison of fwfa callback's early-return logic
     4. Test if view_type oscillates between values during X-Plane view transition

### Historical Note

This codebase was last actively maintained around 2015-2018. The original author is "uglyDwarf" (GitHub). The fixes in this session address API changes in:

- GCC (stricter headers)
- Wine 6.0+ (removed internal APIs)
- Modern autotools

---

## 8. Next Steps (Future Work)

The user mentioned "there will be more later." Potential future work could include:

1. **Testing & Validation**
   - Test with actual TrackIR hardware
   - Test Wine bridge with games
   - Verify X-Plane plugin functionality

2. **Code Modernization**
   - Update remaining format specifiers
   - Consider C++11/14 modernization
   - Add unit tests

3. **Build System**
   - Consider CMake migration
   - CI/CD pipeline setup

4. **Documentation**
   - Update wiki
   - Document Wine prefix setup

---

## 9. Files Created/Modified

### Created

| File | Purpose |
|------|---------|
| `HANDOFF.md` | This document |
| `COMPILATION_FIXES.md` | Technical fix details |
| `INSTALLATION_PATH_ANALYSIS.md` | **CRITICAL:** RPATH and installation path analysis |
| `MODERNIZATION_ROADMAP.md` | Future improvements roadmap (Qt6, CI/CD, etc.) |

> ⚠️ **CRITICAL:** If linuxtrack compiles but doesn't track in games, read `INSTALLATION_PATH_ANALYSIS.md` first! The build system has complex path dependencies involving RPATH, compile-time macros, and runtime configuration.

### Modified

| File | Change |
|------|--------|
| `src/qt_gui/wine_launcher.cpp` | Removed `WINEARCH=win32` to fix 64-bit registry injection |
| `src/CMakeLists.txt` | Complete CMake build system implementation |
| `src/wine_bridge/client/rest.c` | Fixed format specifier for `GetLastError()` |
| `src/ltr_srv_master.cpp` | Added `#include <pthread.h>` |
| `src/pref.cpp` | Added `#include <pthread.h>` |
| `src/wine_bridge/client/NPClient_main.c` | Wine 6.0+ compatibility |

---

## 9.5 UDP Bridge User Guide

### Startup Workflow

> ⚠️ **Follow these steps in order:**

1. **Start the camera** in the Tracking Window (click Start button)
2. **Start UDP Stack** from the UDP Bridge Settings dialog
3. **Start your game**

### Accessing UDP Settings

**Misc tab → Enable UDP Bridge → Configure UDP...**

### Settings Reference

| Setting | Description |
|---------|-------------|
| Target IP | Usually `127.0.0.1` for local games |
| Port | Default `4242` (OpenTrack standard) |
| Protocol | OpenTrack (6 doubles) or FreeTrack |
| Auto-start | Start UDP when tracking begins |

### Controls

| Button | Action |
|--------|--------|
| Start UDP Stack | Spawns `ltr_udp`, optionally starts hotkey utility |
| Stop UDP Stack | Terminates all UDP processes |
| Recenter | Sends recenter command |
| Pause/Resume | Toggles tracking pause |

### Troubleshooting

```bash
# Verify ltr_udp is running
ps -e | grep ltr

# Check for UDP traffic
ss -ulnp | grep 4242
```

### Per-Game Hotkey Customization

When you install the UDP Bridge to a Wine prefix, the current hotkey settings are copied to:

```
<prefix>/drive_c/Program Files/Linuxtrack/ltr_hotkeys.ini
```

**To customize hotkeys for a specific game:**

1. Navigate to the game's Wine prefix
2. Edit `drive_c/Program Files/Linuxtrack/ltr_hotkeys.ini`:

   ```ini
   [Hotkeys]
   Recenter=121   ; 121 = F10 (VK_F10)
   Pause=120      ; 120 = F9 (VK_F9)
   ```

**Common VK codes:**

| Key | VK Code |
|-----|---------|
| F9  | 120     |
| F10 | 121     |
| F11 | 122     |
| F12 | 123     |
| Pause | 19    |

---

## 10. Git Status

### Repository Status

**Branch:** `main` (Default)
**Tag:** `v1.1.12` (Latest Release)
**Status:** Clean (All changes committed and pushed)

```
On branch main
Your branch is up to date with 'origin/main'.
nothing to commit, working tree clean
```

---

## 11. Contact & Resources

- **Original Project:** <https://github.com/uglyDwarf/linuxtrack>
- **Wiki:** <https://github.com/uglyDwarf/linuxtrack/wiki>
- **Issue Tracker:** <https://github.com/uglyDwarf/linuxtrack/issues>

---

### Manual Hotkey Configuration (VK Codes)

If editing `ltr_hotkeys.ini` manually, use these decimal codes:

| Key | Code | Key | Code |
| :--- | :--- | :--- | :--- |
| **F1-F12** | 112-123 | **Pause** | 19 |
| **Home** | 36 | **Insert** | 45 |
| **Delete** | 46 | **End** | 35 |
| **PageUp** | 33 | **PageDn** | 34 |

*End of Handoff Document*
