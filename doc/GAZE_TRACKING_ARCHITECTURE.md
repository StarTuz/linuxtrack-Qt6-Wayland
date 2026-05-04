# Gaze Tracking — Architecture

**Status:** Design. No implementation yet.
**Companion:** [GAZE_TRACKING_ROADMAP.md](GAZE_TRACKING_ROADMAP.md) — phasing, decisions, checklist.
**Last reviewed against:** Linuxtrack 1.4.10.

This document describes how gaze tracking fits into Linuxtrack's existing data flow, ABI surface, and IPC layer. It is the technical reference for everything the roadmap proposes.

---

## 1. Current data flow (1.4.10)

```text
                  ┌─────────────────────────────────────┐
                  │ Tracking sources (one active)       │
                  │  • TrackIR (libtir.so)              │
                  │  • Webcam blobs (libwc.so)          │
                  │  • Webcam neural (libft.so) ────────┼── ONNX: head-localizer + head-pose
                  │  • Wiimote, Joystick, etc.          │
                  └────────────────┬────────────────────┘
                                   │ struct frame_type { bloblist_type ... }
                                   ▼
                  ┌─────────────────────────────────────┐
                  │ Core engine (libltr.so)             │
                  │  src/tracking.c, axis.c, pose.c     │
                  │   - update_absolute_pose()          │
                  │   - axis curves, smoothing          │
                  │   - one_euro_filter                 │
                  └────────────────┬────────────────────┘
                                   │ linuxtrack_full_pose_t (shared mem)
                                   ▼
                  ┌─────────────────────────────────────┐
                  │ ltr_server1 (master daemon)         │
                  └────────────────┬────────────────────┘
                                   │ IPC
                  ┌────────────────┴────────────────────┐
                  ▼              ▼               ▼      ▼
              xlinuxtrack9   Wine bridge    ltr_udp   liblinuxtrack
              (X-Plane)      (NPClient)     (OT UDP)  (3rd-party)
```

**Two ML models already run** in the neural face tracker (`HAVE_ONNXRUNTIME=ON` only):
- `head-localizer.onnx` — bounding box of face.
- `head-pose-0.4-{big-int8|small-f32}.onnx` — 6DoF pose from face crop.

Both shipped under `share/linuxtrack/models/` per [CMakeLists.txt:280-285](../CMakeLists.txt#L280-L285).

---

## 2. Proposed data flow (post-roadmap)

```text
        Existing webcam neural path                    NEW Phase 1 path
   ┌─────────────────────────────────┐         ┌──────────────────────┐
   │ webcam → libft.so               │         │ External producer    │
   │  localizer → head-pose          │         │  • Beam iOS          │
   │      ↓ NEW Phase 3              │         │  • OpenTrack desktop │
   │  → gaze-estimator (L2CS/Mobile) │         │  • smartphone tracker│
   └────────────────┬────────────────┘         └──────────┬───────────┘
                    │                                     │ UDP
                    │                          ┌──────────▼───────────┐
                    │                          │ libnet_input.so      │
                    │                          │  src/net_input_      │
                    │                          │  driver.c            │
                    │                          └──────────┬───────────┘
                    └──────────────┬──────────────────────┘
                                   │ frame_type + gaze fields
                                   ▼
                  ┌─────────────────────────────────────┐
                  │ libltr.so                           │
                  │  + gaze axes (smoothed, mapped)     │
                  │  + calibration → screen coords      │
                  └────────────────┬────────────────────┘
                                   │ linuxtrack_full_pose_t
                                   │   + gaze_yaw, gaze_pitch  (NEW internal fields)
                                   ▼
                  ┌─────────────────────────────────────┐
                  │ ltr_server1                         │
                  └────────────────┬────────────────────┘
                                   │
            ┌──────────────────────┼──────────────────────┐
            ▼                      ▼                      ▼
   xlinuxtrack9 (Phase 0A)   Tobii shim (0B)      OpenTrack-ext (0C)
   gaze DataRefs             impersonate Tobii    UDP +8 bytes
```

New components (all roadmap phases together):
- **`libnet_input.so`** — Phase 1 input driver.
- **Gaze estimator stage** in `libft.so` — Phase 3.
- **Calibration math** in core engine — Phase 2.
- **Destination shim(s)** — Phase 0/4, picked from roadmap §3 Phase 0 options.

---

## 3. Component map

| Component | File(s) | Phase | Role |
|---|---|---|---|
| `GazeEstimator` class | [src/neuralnet_tracker.cpp](../src/neuralnet_tracker.cpp), `.h` | 3 | Owns ONNX session for gaze model. |
| Gaze filtering | [src/facetrack.cpp](../src/facetrack.cpp) | 3 | Two `one_euro_filter` instances (yaw, pitch). |
| Internal pose | [src/ltlib.h](../src/ltlib.h) | 3 | Add `gaze_yaw`, `gaze_pitch` floats to `linuxtrack_full_pose_t`. |
| Public getter | [src/linuxtrack.h](../src/linuxtrack.h) | 3 | New `linuxtrack_get_gaze()`. **No struct changes.** |
| UDP input driver | `src/net_input_driver.c` (new) | 1 | `cal.h` interface, OpenTrack listener. |
| Calibration storage | `~/.config/linuxtrack/`, mINI | 2 | Camera-screen geometry, per-user offsets. |
| Calibration UI | `src/qt_gui/` | 2 | 5/9-point gaze-target sequence. |
| X-Plane gaze DataRefs | [src/xlinuxtrack9.c](../src/xlinuxtrack9.c) | 0A / 4 | Tier-2 file — see [CLAUDE.md](../CLAUDE.md). |
| Tobii shim DLL | `src/wine_bridge/tobii/` (new) | 0B / 4 | Optional, large effort. |
| Regression tests | [src/tests/test_tracking.cpp](../src/tests/test_tracking.cpp), [test_image_process.cpp](../src/tests/test_image_process.cpp) | 3 | TrackIR-cannot-regress. |

---

## 4. ABI / IPC rules

There are **three distinct ABI surfaces** in Linuxtrack. Treat them differently.

### 4.1 Public client C API — frozen

Anything declared in [src/linuxtrack.h](../src/linuxtrack.h) is linked by external clients (X-Plane plugin, Wine bridge DLLs, third-party games using `liblinuxtrack`).

**Rule:** Do not change struct layouts. Adding a field to `linuxtrack_pose_t` changes `sizeof()` and breaks every binary compiled against the previous layout.

**Allowed changes:**
- Append new functions: `int linuxtrack_get_gaze(float *gaze_yaw, float *gaze_pitch);`
- Append new enum values *at the end* of existing enums.

**Disallowed without a major-version bump:**
- Adding fields to `linuxtrack_pose_t` or `linuxtrack_abs_pose_t`.
- Changing function signatures.
- Reordering enum values.

### 4.2 Internal IPC (master ↔ slaves) — versioned but mutable

`linuxtrack_full_pose_t` in [src/ltlib.h](../src/ltlib.h) lives in shared memory between `ltr_server1` and every slave (`xlinuxtrack9`, `ltr_udp`, Wine bridge, etc.). Layout:

```c
#define MAX_BLOBS 10
#define BLOB_ELEMENTS 3   // (x, y, score) per blob

typedef struct {
  linuxtrack_pose_t prev_pose;
  linuxtrack_pose_t pose;
  linuxtrack_abs_pose_t prev_abs_pose;
  linuxtrack_abs_pose_t abs_pose;
  float blob_list[BLOB_ELEMENTS * MAX_BLOBS];   // 30 floats
  // ... gaze fields go here (Phase 3)
} linuxtrack_full_pose_t;
```

**Rule:** May grow. Append new fields at the end. Every slave rebuilds in the same release. This is internal — no external client links against this struct.

### 4.3 UDP wire format (`ltr_udp`, OpenTrack consumers) — mutable with care

OpenTrack format is 6 doubles (48 bytes): `x, y, z, yaw, pitch, roll`. Extending it for gaze means picking option C of Phase 0 — coordinate with OpenTrack upstream, document the variant flag, and keep a mode switch so vanilla OpenTrack consumers keep working.

### 4.4 The "4th blob" misunderstanding

Earlier proposals suggested packing gaze into `blobs[3]` of the bloblist. **This is not necessary** — the wire already carries `MAX_BLOBS=10`, so adding a 4th blob does not break IPC.

**But it is still wrong**, because:
- Blob slots are positional `(x, y, score)` triplets consumed by [src/tracking.c](../src/tracking.c) and the TrackIR 3-point solver. Stuffing angular gaze into a blob slot means every blob consumer must learn the slot is special.
- The blob path has shipped regressions before (v1.3.5 webcam-blob-scaling broke TrackIR). Don't revisit that mine.

**Correct approach:** explicit `gaze_yaw` / `gaze_pitch` fields on `linuxtrack_full_pose_t`. Cleaner, auditable, no risk to the pose solver.

---

## 5. Frame budget & latency

The neural webcam path runs:
1. `head-localizer.onnx` on the full frame.
2. `head-pose.onnx` on the face crop.

Adding stage 3 (gaze) means a third sequential ONNX session per frame. L2CS-Net at 448×448 is the heaviest of the three.

**Required before merge:**
- Baseline: measure end-to-end tracking latency in 1.4.10 on (a) integrated-GPU laptop, (b) mid-range desktop. Without this, "third stage will cost X ms" is unverifiable.
- Three-stage benchmark on the same hardware.
- Hard floor: **30 fps face tracking with all three stages active.** Below that, the feature is a regression.

If the budget is tight: MobileGaze is lighter than L2CS-Net; running gaze every Nth frame (with the filter interpolating) is an option.

---

## 6. Calibration math

Raw gaze model output is angular: pitch and yaw of the eye in **head-relative** coordinates. For a game (or X-Plane DataRef) to use it, we need **screen-space** gaze — the point on the user's monitor they're looking at.

Conversion needs:
1. **Camera-screen geometry** — where is the camera relative to the screen, and what are the screen dimensions in cm?
2. **Camera intrinsics** — FoV / focal length (already known from existing webcam tracking).
3. **Head pose** — already produced by stage 2 of the pipeline.
4. **Per-user offset** — kappa angle (visual axis ≠ optical axis), captured during calibration.

Sketch:

```text
gaze_world_dir = R_head · gaze_head_dir              // rotate head-relative gaze to world frame
ray_origin     = head_position_world
screen_hit     = intersect(ray_origin, gaze_world_dir, screen_plane)
screen_uv      = project(screen_hit, screen_geometry) ∈ [0,1]²
screen_uv     += per_user_offset                     // residual after calibration
```

Per-user offset captured by a 5- or 9-point gaze-target sequence in `ltr_gui` (Phase 2). Store in `~/.config/linuxtrack/gaze_calibration.ini`.

**This is why calibration is Phase 2, not Phase 4:** without it, you have no test harness to validate that the eventual neural gaze pipeline (Phase 3) is producing correct values. Mock gaze input from Phase 1 + calibration from Phase 2 = working end-to-end pipeline ready for the real model to slot in.

---

## 7. Model options

| Model | Paper / Source | Accuracy | Pretrained ONNX? | Notes |
|---|---|---|---|---|
| **L2CS-Net** | [arXiv:2203.03339](https://arxiv.org/abs/2203.03339) | 3.92° MPIIGaze | Conversion required | Two-branch CNN; well-cited; heaviest of the three. |
| **MobileGaze** | [github.com/yakhyo/gaze-estimation](https://github.com/yakhyo/gaze-estimation) | Lightweight | **Claims pretrained — verify** | Mobile-targeted; lightest; good first candidate *if* weights are real. |
| **GazeTR** | [ICPR 2022](https://github.com/yihuacheng/GazeTR) | High | Conversion required | Transformer; heavy; not a first choice. |
| **ETH-XGaze** | [ait.ethz.ch/xgaze](https://ait.ethz.ch/xgaze) | — | Dataset, not a model | Useful for training/validation if we ever roll our own. |

**Default candidate:** MobileGaze, *contingent on verifying its pretrained ONNX is real and loadable*. Fallback: L2CS-Net (accept the conversion cost).

This verification is a roadmap checklist item — see [GAZE_TRACKING_ROADMAP.md §4](GAZE_TRACKING_ROADMAP.md#4-decision-checklist-before-any-code-lands).

---

## 8. Competitive context (why bother)

Brief — the full marketing comparison is no longer maintained, but the technical context is:

| | Tobii Eye Tracker 5 | Eyeware Beam | Linuxtrack (planned) |
|---|---|---|---|
| Sensor | Dedicated 850nm IR | iPhone TrueDepth / webcam | Webcam |
| Algorithm | PCCR (corneal glint, hardware) | Neural net (proprietary) | Neural net (L2CS / MobileGaze) |
| Calibration | Factory + per-user | Per-session | Per-user (Phase 2) |
| Platform | Windows only | Windows + iOS | Linux (primary), macOS (experimental) |
| Linux gaming consumer | None | Via iOS+UDP relay | Native + UDP-input driver |
| Cost | ~$250 | ~$30 | Free |

**Tobii's real advantage is hardware** (IR + corneal reflection), not algorithms. Webcam gaze can approach but not match its precision. **Beam's real advantage is the Tobii Game Hub bridge** — it impersonates a Tobii tracker so games with native Tobii integration just work. That bridge is the consumer-side missing piece, captured in [Roadmap Phase 0 option B](GAZE_TRACKING_ROADMAP.md#phase-0--pick-destination-protocol-prerequisite-no-code-in-14-lands-until-this-is-done).

Without solving the destination problem, "feature parity with Beam" is not an achievable goal — it is two unrelated halves: the inference pipeline (which we can build) and the protocol bridge (which is the actual user-visible value). The roadmap addresses both.

---

## 9. Open technical questions

Same list as the roadmap §6, restated here for ergonomics:

1. **Tobii Stream Engine impersonation:** does TGH/Tobii's auth surface block third-party shim DLLs? Affects Phase 0B viability.
2. **Beam iOS UDP format:** documented or proprietary? Affects Phase 1 — if undocumented, OpenTrack is the realistic input target.
3. **Existing pipeline latency:** unknown on representative hardware. Blocks Phase 3 budget claim.
4. **Per-profile vs. global gaze:** affects `CMD_PROFILE_CHANGE` IPC handler design ([CLAUDE.md](../CLAUDE.md) IPC rules).
5. **MobileGaze pretrained weights reality check:** roadmap-gating.
