# Technical Proposal: Native Gaze Tracking Integration (Phase 1)

This document outlines the proposed architecture for integrating native neural gaze estimation into Linuxtrack. This is intended for review and comment by the community and other agents.

> **REVIEW (2026-05-01, against v1.4.10):** Overall the architecture is reasonable but the **phase ordering is inverted**. Phases 1–2 build inference and transport before Phase 3 confirms a real game-side consumer for the gaze data. Recommend pinning down at least one concrete protocol target (Tobii Game Hub bridge, native gaze-aware UDP protocol, etc.) **before** implementation begins, otherwise we ship gaze data with no consumer. See per-section comments below.

## 1. Objective
Extend Linuxtrack's neural tracking pipeline to include gaze yaw and pitch estimation. This data will be captured from the camera feed, processed via a specialized ONNX model, and transmitted through Linuxtrack's standard UDP data stream for use in games and other applications.

> **REVIEW — destination unspecified:** "Linuxtrack's standard UDP data stream" today is OpenTrack 6-doubles or NPClient (Wine bridge). Neither carries gaze. The NPClient/TrackIR protocol is strictly 6DoF — there is no extension point for gaze in the wire format that games consume. The objective should be rewritten to name the *target protocol* (and therefore the target games) rather than implying the existing UDP stream will carry it.

## 2. Proposed Architecture

### 2.1 Sequential Inference Pipeline
The tracking pipeline will be updated to run three models in sequence for each frame:
1. **Face Localizer**: Identifies the bounding box of the face.
2. **Head Pose Estimator**: Calculates the 6DoF head pose from the face crop.
3. **Gaze Estimator (New)**: Calculates angular gaze vectors (yaw/pitch) from the same face crop.

### 2.2 Data Structure Extensions
To accommodate the new data, we propose the following changes to core headers:
- **`linuxtrack_pose_t`**: Add `gaze_yaw` and `gaze_pitch` fields.
- **`linuxtrack_abs_pose_t`**: Add `abs_gaze_yaw` and `abs_gaze_pitch` fields.

> **REVIEW — ABI break, blocker for any 1.x release:**
> `linuxtrack_pose_t` lives in [src/linuxtrack.h](../src/linuxtrack.h), the **public C API** that external clients link against (X-Plane plugin, Wine bridge DLLs, OpenTrack consumers, third-party games). Appending fields to a public struct changes its `sizeof()`, which breaks every binary compiled against the old layout. This is a `2.0.0` change, not `1.4.11` or `1.5.0`.
>
> **Recommended alternative:** Keep `linuxtrack_pose_t` and `linuxtrack_abs_pose_t` frozen. Add a sibling C API: `int linuxtrack_get_gaze(float *gaze_yaw, float *gaze_pitch);` — internal structs (`linuxtrack_full_pose_t` etc.) can carry the data; the public surface stays additive-only. This is the standard way to extend a 1.x C library.

### 2.3 UDP Transmission (Blob Packaging)
Linuxtrack's current `bloblist` system uses 3 blobs of 3 floats each. To maintain backward compatibility while adding gaze data, we propose:
- Utilizing a **4th blob** (`blobs[3]`) to transport `gaze_yaw` and `gaze_pitch`.
- Existing clients will ignore the 4th blob, while updated clients (and the tracking core) will unpack it to extract gaze information.

> **REVIEW — verify wire format claim:** "Existing clients will ignore the 4th blob" is only true if the IPC payload already carries a variable blob count. If `MAX_BLOBS=3` is hardcoded in the IPC struct size (likely; check [src/ipc_utils.c](../src/ipc_utils.c) and the `bloblist_type` / `linuxtrack_full_pose_t` definitions), bumping to 4 is **itself an IPC ABI break** — every running slave (`ltr_udp`, `xlinuxtrack9`, the Wine bridge) would need a synchronized rebuild. This must be confirmed with code references before this approach can be called "backward compatible."
>
> Even if it works, packing two angular values into a struct meant for `(x, y, score)` blob detections is semantically lossy. Cleaner option: route gaze through a dedicated path in the IPC layer (new message type, new field on the internal pose struct) rather than overloading the blob list.

## 3. Detailed Implementation Plan

### 3.1 Neural Pipeline (`src/neuralnet_tracker.h/cpp`)
- Implement a `GazeEstimator` class to handle ONNX inference for gaze.
- Update `NeuralNetTracker::detect()` to execute the `GazeEstimator` and return the smoothed results.

> **REVIEW — performance and availability:** Two concerns. **(a) Latency budget unaddressed** — a third sequential ONNX session per frame (Localizer → PoseEstimator → GazeEstimator) on top of the existing two will cost real time. L2CS-Net at 448×448 is the heaviest of the three. Benchmark on representative hardware (integrated GPU laptop, mid-range desktop) before locking in the architecture; if face tracking drops below 30 fps the feature is a regression by default. **(b) `HAVE_ONNXRUNTIME=OFF` is the common case** — most distros do not ship `onnxruntime` (recent EndeavourOS source build logged `ONNXRuntime not found - neural webcam face tracking disabled`). Gaze tracking will be unavailable to anyone building from source without manually installing the runtime. Document this and mention the AppImage as the supported delivery vehicle for gaze tracking.

### 3.2 Filtering & Orchestration (`src/facetrack.cpp`)
- Initialize `one_euro_filter` instances for both gaze axes to ensure stable, jitter-free tracking.
- Pack the filtered gaze values into `blobs[3]` of the `bloblist_type` structure.

### 3.3 Core Unpacking (`src/tracking.c`)
- Update `update_absolute_pose()` to detect and unpack the 4th blob from the incoming data stream.
- Map these values to the internal `current_pose` state.

> **REVIEW — risk to TrackIR/webcam tracking:** [src/tracking.c](../src/tracking.c) is the shared blob → pose path used by both the neural webcam tracker and the TrackIR 3-point solver. v1.3.5 already carries scars from this exact area (the webcam blob-scaling regression that broke TrackIR — see HANDOFF.md sections on v1.3.5/v1.3.7). Any change to `update_absolute_pose()` or blob unpacking semantics needs explicit regression coverage in `src/tests/test_tracking.cpp` and `src/tests/test_image_process.cpp` covering: 3-blob input still produces an unchanged pose, 4-blob input does not perturb the 3-point solver, and the TrackIR path never touches `blobs[3]`.

## 4. Future Phases
- **Phase 2**: Validation and testing of the 4th blob transmission.
- **Phase 3**: Updating the `NPClientUDP` bridge to map gaze values to game-specific protocols.
- **Phase 4**: GUI integration for gaze visualization and calibration.

> **REVIEW — Phase 3 is the load-bearing one:** The current ordering builds inference (Phase 1) and transport (Phase 2) before confirming the destination (Phase 3). NPClient cannot carry gaze — its wire protocol is defined by the TrackIR DLL signature and consumed by closed-source games. Updating "the `NPClientUDP` bridge to map gaze values to game-specific protocols" is hand-wavy; in practice, target games either accept Tobii's protocol (Star Citizen, DCS, Elite Dangerous have native Tobii integration) or they don't accept gaze at all. **Recommend reordering:** Phase 1 = pick destination protocol and prototype it with mock data; Phase 2 = ONNX integration; Phase 3 = calibration UI. Calibration also can't be deferred to "Phase 4" — raw angular gaze is unusable without screen-space mapping, so calibration gates real validation.

---
**Reviewers**: Please comment on the feasibility of the 4th blob approach and the sequential model pipeline.

> **REVIEW — summary checklist before implementation:**
>
> - [ ] Identify and prototype the target gaze-consumer protocol (Tobii Game Hub bridge? new gaze-aware UDP protocol? Linuxtrack-only client?). No code in Phase 1/2 should land before this is concrete.
> - [ ] Confirm whether the IPC `bloblist_type` already carries 4 blobs on the wire, or whether expanding to 4 is itself an IPC ABI break for slaves.
> - [ ] Replace `linuxtrack_pose_t` field additions with a sibling `linuxtrack_get_gaze()` API to avoid breaking the public C ABI.
> - [ ] Benchmark three-stage ONNX pipeline on representative hardware; document the fps impact.
> - [ ] Add `tests/test_tracking.cpp` and `tests/test_image_process.cpp` regression coverage so the 4th-blob path cannot regress the TrackIR 3-point solver.
> - [ ] Decide whether to land this on master (current 1.4.x line) or a feature branch targeting `2.0.0`. Adding ABI surface in 1.4.x is not viable.
