# Webcam Support Audit

## Scope

This note summarizes the current Linux webcam stack in Linuxtrack and the main reasons recent regressions were easy to introduce.

## Current Flow

1. The Qt device setup UI enumerates every V4L2-advertised format, resolution, and frame interval.
2. The selected mode is persisted through `wc_driver_prefs`.
3. `libwc` / `libft` load the saved mode and configure V4L2 in `src/webcam_driver.c`.
4. Frames are converted to a thresholded grayscale bitmap.
5. Blob tracking consumes that bitmap directly, while face tracking passes it into OpenCV.
6. The "Camera View" tab displays the processed bitmap, not a raw camera preview.

## Structural Problems

- UI/runtime mismatch: the UI offers modes the runtime path may not decode or process efficiently.
- Preview ambiguity: the camera tab is a diagnostic mask for webcam/blob tracking, not a normal preview.
- Shared driver complexity: blob tracking and face tracking both use `src/webcam_driver.c`, but with materially different expectations.
- Weak observability: the app does not surface negotiated source format, active stream interval, conversion path, or processing bottlenecks clearly enough.
- Missing tests: the existing webcam test only exercises enumeration; it does not validate live capture, restart, conversion, or frame rate behavior.

## Recommended Direction

- Define supported webcam modes explicitly for:
  - blob tracking
  - face tracking
  - converted/compressed modes
- Split raw preview from processed tracking preview in the UI.
- Add runtime diagnostics:
  - requested format
  - negotiated format
  - active driver interval
  - processing fps
  - preview mode
- Add a real webcam smoke test target that can validate:
  - enumeration
  - start/stop/restart
  - delivered frame dimensions
  - negotiated frame interval

## Immediate Safe State

- Keep the installer fix that always rebuilds before install/upgrade.
- Do not push experimental webcam conversion changes until the subsystem redesign is defined and validated on hardware.
