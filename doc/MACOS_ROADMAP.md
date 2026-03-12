# Linuxtrack macOS Roadmap

**Date:** 2026-03-11  
**Status:** Phase 0-1 complete, Phase 2 in progress
**Scope:** Experimental future platform bring-up after current Linux stabilization work

---

## 1. Goal

Bring Linuxtrack to modern macOS without destabilizing the Linux product.

This is not a maintenance task. The existing macOS code in the tree is historical and should be treated as reference material, not as a supported implementation.

The first real target should be:

- Qt GUI on macOS
- webcam and face tracking
- experimental packaging for Intel and Apple Silicon Macs

TrackIR hardware and X-Plane plugin support should be planned as later gates, not assumed.

---

## 2. Current State

There are still macOS remnants in the repository:

- `src/mac/`
- `src/macwebcam_driver.c`
- `src/macwii_driver.c`
- `src/osc_server_mac/`
- macOS-specific Qt preference panes under `src/qt_gui/`

However:

- modern CMake does not build a supported macOS application path
- the old camera path uses QTKit-era code and should not be revived as-is
- there is no current macOS CI
- there is no modern macOS packaging, signing, or notarization path

Conclusion:

- the shared tracking core is reusable
- the macOS platform layer is effectively a new bring-up

---

## 3. Guardrails

These rules exist to avoid breaking Linux while macOS work is in progress.

1. All macOS work stays behind a dedicated CMake option such as `BUILD_MAC_EXPERIMENTAL`, default `OFF`.
2. Shared-core refactors must land with regression tests before any mac-only feature depends on them.
3. No Linux behavior changes are allowed without Linux regression coverage.
4. New macOS code should live behind narrow platform interfaces instead of spreading `#ifdef APPLE` through shared tracking logic.
5. No macOS release artifacts should be published from the normal release path until dedicated macOS CI is green.

---

## 4. Delivery Phases

### Phase 0: Containment

**Status:** COMPLETE (2026-03-11)

Purpose:

- make room for macOS work without affecting the existing Linux build graph

Implementation:

- add `BUILD_MAC_EXPERIMENTAL`
- isolate old macOS sources from normal Linux targets
- define explicit platform seams for camera input, device enumeration, and packaging

Required tests:

- Linux compile still works with macOS option disabled
- existing CTest suite still passes unchanged

Exit criteria:

- Linux build graph is identical when `BUILD_MAC_EXPERIMENTAL=OFF`
- mac-specific GUI scaffolding is excluded from the default Linux `ltr_gui` target

Completed in repo:

- added `BUILD_MAC_EXPERIMENTAL` in the top-level CMake configuration
- moved mac-specific Qt GUI sources, forms, and headers behind `APPLE AND BUILD_MAC_EXPERIMENTAL`
- changed `device_setup.cpp` to use `LTR_MAC_EXPERIMENTAL` instead of assuming all Darwin-targeted code belongs in the default build

Verification run:

- `cmake -S . -B build_phase0_verify`
- `cmake --build build_phase0_verify --target ltr_gui -j"$(nproc)"`
- `cmake --build build_phase0_verify --target test_lal test_webcam_info test_image_process test_wc_driver_prefs -j"$(nproc)"`
- `ctest --test-dir build_phase0_verify --output-on-failure`

### Phase 1: Shared-Core Hardening

**Status:** COMPLETE (2026-03-12)

Purpose:

- ensure the tracking pipeline is testable without hardware and portable across platforms

Implementation:

- separate frame-source code from blob extraction and pose solving
- separate platform device enumeration from preferences and UI logic
- keep filters, pose solving, image processing, and config parsing platform-neutral

Required regression tests:

- shared blob extraction semantics
- 3-point pose invariants
- face-derived translation and smoothing behavior
- config defaulting and clamping for `MacWebcam` and `MacWebcam-face`

Recommended harnesses:

- synthetic blob-frame fixtures
- recorded pose and landmark replay inputs
- preference round-trip tests

Exit criteria:

- core tracking tests run without real hardware
- Linux webcam and TrackIR behavior is unchanged

Completed so far:

- added `test_pref_global` to lock down device-category mapping for `MacWebcam`, `MacWebcam-face`, `Ps3Eye`, and `Ps3Eye-face`
- added `test_tracking` to lock down shared `tracking.c` behavior for:
  - single-point face `tz` estimation from face-size changes
  - face-tracker routing to the absolute-pose path when 3 blobs are present
  - 3-point solver failure not advancing the pose counter
  - translation passthrough vs. alignment in `ltr_int_postprocess_axes()`
  - rejection of non-finite axis values before filtering/alignment
  - camera-orientation normalization before 3-point pose solving
  - `ORIENT_FROM_BEHIND` sign handling for solved 3-point pose values
- extracted camera-orientation normalization into explicit shared helpers in `src/tracking.h` / `src/tracking.c` so future frame sources can reuse the same semantics without duplicating private tracking internals
- extracted pose-route selection into an explicit shared helper in `src/tracking.h` / `src/tracking.c` so the decision between single-point, absolute, and 3-point updates is testable and no longer buried as ad hoc branching inside `ltr_int_update_pose()`
- extracted frame snapshot copying into an explicit shared helper in `src/tracking.h` / `src/tracking.c` so future frame sources can populate `linuxtrack_full_pose_t` without depending on the internal lock/update flow inside `ltr_int_update_pose()`
- extended `test_wc_driver_prefs` so `MacWebcam-face` shares the same camera FOV defaults and clamping behavior as `Webcam-face`
- fixed a real shared-core bug in `src/tracking.c` where `ltr_int_update_pose()` stored its result in a `bool`, collapsing `-1` failure/discard returns into `1`
- added direct `test_tracking` coverage for the shared frame snapshot helper so blob coordinates, scores, and frame resolution semantics stay fixed across later platform work

Verification run:

- `cmake --build build_phase0_verify --target test_pref_global test_tracking test_wc_driver_prefs -j"$(nproc)"`
- `ctest --test-dir build_phase0_verify --output-on-failure`

### Phase 2: macOS App Shell

**Status:** IN PROGRESS (2026-03-12)

Purpose:

- get a macOS application bundle to build and launch without promising working tracking yet

Implementation:

- modern CMake app target for macOS
- Qt GUI build on macOS
- app bundle metadata and `Info.plist`
- camera permission plumbing
- separate experimental workflow for macOS builds

Required tests:

- compile-only CI job on macOS
- app-launch smoke test
- settings load/save smoke test
- bundle content verification

Exit criteria:

- `.app` launches on a tester machine
- preferences persist
- Linux CI remains unaffected

Completed so far:

- `src/qt_gui/CMakeLists.txt` now builds `ltr_gui` as a `MACOSX_BUNDLE` when `APPLE AND BUILD_MAC_EXPERIMENTAL`, while leaving Linux as the existing plain executable target
- added `src/qt_gui/Info.plist.in` so the experimental mac app has explicit bundle metadata, versioning, icon, and camera-permission text under CMake instead of relying on the old autotools-era mac scaffolding
- attached the existing `src/qt_gui/linuxtrack.icns` to the mac bundle resources and switched the experimental mac output name to `Linuxtrack.app`
- taught `PrefProxy::getDataPath()` to resolve bundle-relative `Contents/Resources/linuxtrack` assets first on macOS so help files, default prefs, and extractor metadata can work from the app bundle instead of assuming a Linux-style `share/linuxtrack` layout
- embedded the GUI data files, `linuxtrack1.conf`, `sources_mac.txt`, and Qt help files into the experimental mac bundle resource tree so later smoke tests can run against a self-contained app layout
- updated install rules so experimental mac builds install the bundle as an app while Linux continues to install `ltr_gui` into `bin/`
- isolated obvious Linux-only defaults from the experimental mac configure path by force-disabling Mickey and the Wine bridge under `APPLE AND BUILD_MAC_EXPERIMENTAL`, skipping Linux desktop metadata installs on macOS, and keeping Linux-only hotkey/plugin subdirectories out of the mac target graph
- added `scripts/configure_macos_experimental.sh` as the canonical Phase 2 configure entry point for future mac testers; it enables `BUILD_MAC_EXPERIMENTAL`, disables known non-mac targets, turns off the neural tracker for the app-shell phase, and uses a repo-local staging prefix instead of `/opt/linuxtrack`
- added `PrefProxy::findRuntimeFile()` / `getExecutablePath()` and switched several GUI helper flows (`ltr_udp`, hotkey daemons, Wine bridge injection, and X-Plane plugin lookup) away from hardcoded `/opt/linuxtrack` paths so they can resolve artifacts from an app bundle, AppImage, install tree, or development build layout
- removed the last GUI-side fixed `/opt/linuxtrack` hotkey lookup in `udp_settings.cpp` and added a Darwin-first fallback in `src/linuxtrack.c` so the public client loader tries `@executable_path`, `@loader_path`, and `@rpath` framework locations before relying on the legacy config-prefix search
- made the top-level install defaults less Linux-specific for the experimental mac path: `CMAKE_INSTALL_PREFIX` now defaults to a repo-local staging directory on `APPLE AND BUILD_MAC_EXPERIMENTAL`, and install RPATHs now use `@loader_path`-style bundle/library locations on macOS while Linux keeps the existing `/opt` + `$ORIGIN` behavior

Verification run:

- `cmake -S . -B build_phase0_verify`
- `cmake --build build_phase0_verify --target ltr_gui -j"$(nproc)"`

Suggested experimental mac configure flow:

- `scripts/configure_macos_experimental.sh`
- `cmake --build build_macos_experimental --target ltr_gui -j"$(sysctl -n hw.ncpu)"`
- `cmake --install build_macos_experimental`

### Phase 3: Webcam and Face Tracking

Purpose:

- deliver the first real end-to-end tracking feature on macOS

Implementation:

- replace the old QTKit capture path with a modern macOS capture layer
- adapt frames into the existing shared image and face tracking pipeline
- reuse the current Linux-side smoothing and face pose improvements where possible

Required regression tests:

- frame timestamp monotonicity
- dropped-frame handling
- pixel format conversion into the shared grayscale/blob path
- macOS webcam preference mapping

Recommended harnesses:

- replay of captured macOS webcam frames into the shared processing path
- deterministic face-pose sample playback for smoothing checks

Manual validation matrix:

- Intel Mac
- Apple Silicon Mac
- built-in camera
- at least one USB camera if available

Exit criteria:

- stable preview and 3D view with webcam input on both architectures

### Phase 4: X-Plane Plugin on macOS

Purpose:

- support the most likely real user demand from the X-Plane community

Implementation:

- add a macOS X-Plane plugin target
- package the plugin in the correct X-Plane directory layout
- ensure correct binary architecture for Intel and Apple Silicon builds

Required tests:

- compile-only plugin job
- plugin artifact layout verification
- fake X-Plane directory installation test

Manual validation:

- X-Plane loads the plugin
- pose updates arrive from the macOS app

Exit criteria:

- webcam-driven tracking works in X-Plane on a tester Mac

### Phase 5: TrackIR on macOS

Purpose:

- evaluate TrackIR hardware support only after the macOS app, webcam tracking, and X-Plane path are already stable

Implementation:

- add a dedicated macOS TrackIR device layer
- keep USB/HID access separate from Linux TrackIR paths
- do not assume current historical code is viable

Required regression tests:

- packet parser tests using captured device data
- pose-solver replay tests from recorded blob streams
- no-device startup and shutdown tests

Recommended harnesses:

- raw packet capture logs from the tester machine
- blob replay into the existing 3-point solver

Manual validation:

- camera/blob view works
- 3D pose works
- reconnect does not wedge the app

Exit criteria:

- TrackIR works on at least one real macOS tester machine

### Phase 6: Packaging and Support

Purpose:

- make the experimental macOS support installable enough for external testers

Implementation:

- signed `.app`
- `.dmg` or `.pkg`
- document architecture and feature support matrix
- consider notarization if outside-testing expands

Required tests:

- bundle verification
- installer smoke test
- architecture inspection

Exit criteria:

- tester receives a repeatable installable artifact

---

## 5. Feature Priority

Recommended order of practical value:

1. macOS app bundle
2. webcam and face tracking
3. X-Plane plugin support
4. TrackIR hardware support

This is intentionally not ordered by forum demand. TrackIR on macOS is the riskiest technical path and should not be the first deliverable.

---

## 6. Risk Register

### High Risk

- historical macOS camera code is obsolete
- TrackIR hardware support may require substantial new device-access work
- Apple Silicon and Intel both need validation

### Medium Risk

- X-Plane plugin architecture and packaging differences
- macOS bundle/signing/notarization workflow

### Low Risk

- shared-core tracking math reuse
- preference parsing and configuration plumbing
- Qt GUI preference panes already present in the tree

---

## 7. Recommended Starting Work

Do not start with device code first.

Start with:

1. `BUILD_MAC_EXPERIMENTAL` and build-graph isolation
2. additional shared regression tests for pose, face translation, and mac device prefs
3. a compile-only macOS CI job

Only after those are in place should the project touch real macOS capture code.

---

## 8. Tester Expectations

The current external tester should be treated as a day-zero guinea pig, not as a supported user.

The first communication when testing starts should be explicit that:

- early macOS builds are expected to fail
- first builds will likely support webcam before TrackIR
- X-Plane support may lag behind the first macOS GUI build

That expectation is correct and should remain in place until at least Phase 4 is complete.
