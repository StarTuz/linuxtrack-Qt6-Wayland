# Gaze Tracking — Consolidated Roadmap

**Status:** Planning. No implementation has begun.
**Authority:** This document supersedes `GAZE_TRACKING_PROPOSAL.md`, `eyeware_beam_integration_assessment.md`, and `eyeware_beam_comparison.md`. Those files now redirect here.
**Companion:** [GAZE_TRACKING_ARCHITECTURE.md](GAZE_TRACKING_ARCHITECTURE.md) — data flow, ABI rules, model options.
**Last reviewed against:** Linuxtrack 1.4.10.

---

## 1. Why this document exists

Three earlier docs (proposal, integration assessment, competitive comparison) covered the same topic with conflicting phase orderings and unspoken scope assumptions. This roadmap is the single source of truth for any agent, contributor, or reviewer working on gaze tracking.

If you find a contradiction between this file and any other doc in the repo, **this file wins** and the other doc should be updated.

---

## 2. Guiding principles

These are non-negotiable. Any plan that violates them is rejected on contact.

1. **Destination first.** No inference, model, or filtering work lands until at least one concrete *consumer* of gaze data is identified and prototyped with mock input. Building gaze data with no game to receive it is wasted effort.
2. **Public C ABI is frozen.** [src/linuxtrack.h](../src/linuxtrack.h) is linked by external clients (X-Plane plugin, Wine bridge DLLs, third-party games). Adding fields to `linuxtrack_pose_t` or `linuxtrack_abs_pose_t` is a `2.0.0` change and is **not** in scope for the 1.4.x or 1.5.x line. Use sibling getters instead — see [Architecture §4](GAZE_TRACKING_ARCHITECTURE.md#4-abi--ipc-rules).
3. **Internal IPC may evolve, but coherently.** `linuxtrack_full_pose_t` (the shared-mem struct between master and slaves) can grow — but every slave (`ltr_udp`, `xlinuxtrack9`, Wine bridge, etc.) must be rebuilt in the same release. Do not pretend internal-IPC changes are "backward compatible."
4. **Don't overload `bloblist`.** The wire format already carries `MAX_BLOBS=10` blobs of `BLOB_ELEMENTS=3` floats — so adding a "4th blob" doesn't break IPC. But blob slots are positional `(x, y, score)` triplets consumed by [src/tracking.c](../src/tracking.c) and the TrackIR 3-point solver. Stuffing angular gaze into `blobs[3]` requires every blob consumer to learn the slot is special. Instead, extend the pose struct with explicit `gaze_yaw` / `gaze_pitch` fields. Cleaner, auditable, and the wire-size argument is moot.
5. **`HAVE_ONNXRUNTIME=OFF` is the common case.** Most distros do not package `onnxruntime`. Native gaze is unavailable from a typical source build. Document this and treat the AppImage as the supported delivery channel for gaze features.
6. **Calibration is a hard problem, not a Phase 4 deferral.** Raw angular gaze (head-relative pitch/yaw) is unusable in games without screen-space mapping, which needs camera-screen geometry, head pose composition, and per-user offset. Calibration must land *before* gaze ships, not after.
7. **TrackIR cannot regress.** The webcam blob path and the TrackIR 3-point solver share `update_absolute_pose()` in [src/tracking.c](../src/tracking.c). v1.3.5 already shipped a regression here. Any change touching this file requires regression tests in [src/tests/test_tracking.cpp](../src/tests/test_tracking.cpp) and [src/tests/test_image_process.cpp](../src/tests/test_image_process.cpp).

---

## 3. Phase ordering

The phases below are deliberately ordered so that each phase produces something useful on its own, even if later phases never ship.

### Phase 0 — Pick destination protocol *(prerequisite, no code in 1–4 lands until this is done)*

The single most important decision. Without it, everything else outputs to `/dev/null`.

| Option | What it is | Effort | Reach | Risk |
|---|---|---|---|---|
| **A. X-Plane plugin extension** | Extend [src/xlinuxtrack9.c](../src/xlinuxtrack9.c) to expose gaze as additional DataRefs (head-coupled view, foveated-rendering hint). | Low | X-Plane only — but we already own the plugin. | Tier 2 file; tested live with X-Plane per [CLAUDE.md](../CLAUDE.md). |
| **B. Tobii Stream Engine shim** (Wine bridge) | A `tobii_stream_engine.dll` impersonating Tobii so games with native Tobii integration (Star Citizen, DCS, Elite Dangerous) consume Linuxtrack gaze unchanged. | High | Largest game catalog. | Tobii API surface is large; signature/auth checks unknown; legal posture unclear. |
| **C. Gaze-aware OpenTrack extension** | Extend OpenTrack UDP protocol with optional 8 trailing bytes (gaze yaw/pitch). Coordinate with OpenTrack upstream. | Low | Few games consume OpenTrack natively for gaze. | Low — but limited consumer surface. |
| **D. Linuxtrack-only client** | Ship a separate gaze-aware app (e.g., custom recenter, foot-pedal-style helper). | Low | Linuxtrack tooling only. | Low. |

**Recommendation:** Start with **A** (X-Plane). It is scoped, owned by us, and produces a real demo. Pursue **C** in parallel as a low-risk interop baseline. Defer **B** until A+C are validated — it is the highest-reach option but largest engineering and legal risk.

**Exit criteria for Phase 0:**
- [ ] Decision documented in this file.
- [ ] Mock-data prototype exists: gaze yaw/pitch fed manually into the chosen protocol, observed in the chosen consumer (X-Plane DataRef, etc.).
- [ ] No real inference involved at this point.

### Phase 1 — UDP input driver *(parallel-safe with Phase 0)*

Add a tracking-source plugin that ingests pose data from external producers — Eyeware Beam iOS, OpenTrack desktop, smartphone face trackers, anything speaking OpenTrack-over-UDP.

This ships value *immediately*, independent of native gaze:
- iPhone-owning users get a high-quality face tracker on Linux today (Beam uses TrueDepth IR + on-device NN — quality far above any webcam-only path we can build short-term).
- Validates the input plumbing that Phase 3 will eventually feed.
- Zero ML dependencies.

**Tasks:**
1. New driver: `src/net_input_driver.c` → `libnet_input.so`, registered as a tracking source via `cal.h` interface.
2. UDP listener on configurable port, decoding OpenTrack 6-doubles format. Optional extended format with trailing gaze bytes (matches Phase 0 option C if chosen).
3. "Network Input" device type in `ltr_gui` device picker.
4. Port + protocol-variant config in preferences (mINI).
5. Test: feed packets from `nc`/Python script and confirm pose updates in `ltr_server1`.

**Difficulty:** Medium. **Dependencies:** none beyond POSIX sockets.

### Phase 2 — Calibration & screen-space mapping

Gaze yaw/pitch in head-relative coordinates is unusable to a game. Calibration converts angular gaze + head pose + camera geometry → screen coordinates (or normalized `(-1, 1)` screen-space).

**Tasks:**
1. Camera-relative-to-screen geometry: store screen width/height, camera position relative to screen (top-center / bottom-center / offset), camera FoV. Stored in `~/.config/linuxtrack/`.
2. Per-user offset calibration UI: 5- or 9-point gaze-target sequence in `ltr_gui`, capturing residual offset after the geometric transform.
3. Math sketch is in [Architecture §6](GAZE_TRACKING_ARCHITECTURE.md#6-calibration-math).
4. Validate with mock gaze input from Phase 1 (e.g., a Python script that emits known gaze targets) before any neural-net work begins.

**Why this is Phase 2, not Phase 4:** without it, you cannot tell whether a downstream gaze pipeline is correct. Calibration is the test harness for everything that follows.

### Phase 3 — Native ONNX gaze

Add a third stage to the existing neural pipeline in [src/neuralnet_tracker.cpp](../src/neuralnet_tracker.cpp). By this point, the destination protocol works (Phase 0), the input plumbing works (Phase 1), and calibration works (Phase 2). All this phase does is replace mock-or-Beam input with our own model output.

**Tasks:**
1. Pick model — see [Architecture §7](GAZE_TRACKING_ARCHITECTURE.md#7-model-options). Default candidate: MobileGaze (claims pretrained ONNX) with L2CS-Net as fallback. **Verify the pretrained weights are real and ONNX-loadable before committing.**
2. Implement `GazeEstimator` class wrapping the ONNX session.
3. Wire into `NeuralNetTracker::detect()` as a third sequential stage after head-pose.
4. Two `one_euro_filter` instances (one per axis) in [src/facetrack.cpp](../src/facetrack.cpp).
5. Extend internal `linuxtrack_full_pose_t` (in [src/ltlib.h](../src/ltlib.h)) with `gaze_yaw` / `gaze_pitch` floats. **Do not** modify `linuxtrack_pose_t` in [src/linuxtrack.h](../src/linuxtrack.h) — see Principle 2.
6. Add public sibling getter: `int linuxtrack_get_gaze(float *gaze_yaw, float *gaze_pitch);` in `linuxtrack.h` — additive, no struct changes.
7. Benchmark three-stage pipeline (localizer → head-pose → gaze) on representative hardware: integrated-GPU laptop and a mid-range desktop. **If face tracking drops below 30 fps, the feature is a regression** — re-evaluate before merging.
8. Document `HAVE_ONNXRUNTIME=OFF` reality in README and point users at the AppImage.
9. Regression tests for the pose path — see Principle 7.

**Difficulty:** Medium. **Dependencies:** ONNX Runtime (already integrated, optional), pretrained model weights.

### Phase 4 — Game integration & GUI polish

Real consumers wired up against real gaze.

- Phase 0 destination becomes production: X-Plane DataRefs validated in flight; Tobii shim wired up if pursued.
- Gaze visualizer in `ltr_gui` (live yaw/pitch bars, calibration replay).
- Per-profile gaze enable/disable.
- Documentation: README section, troubleshooting, model file paths.

---

## 4. Decision checklist before any code lands

This is the gating list. Do not start Phase 1 implementation until every item is resolved.

- [ ] **Phase 0 destination chosen and prototyped with mock data.** (Recommended: option A, X-Plane plugin DataRefs.)
- [ ] **Public ABI rule confirmed:** `linuxtrack_pose_t` and `linuxtrack_abs_pose_t` will not change; gaze is exposed via sibling getter.
- [ ] **Internal IPC plan confirmed:** `linuxtrack_full_pose_t` gets explicit `gaze_yaw` / `gaze_pitch` fields; all slaves rebuild together. No blob-slot overloading.
- [ ] **MobileGaze pretrained ONNX weights verified to exist and load.** If not, fall back to L2CS-Net (training/conversion required).
- [ ] **Latency target stated and a benchmark plan exists** (target: ≥30 fps on integrated-GPU laptop with all three stages active).
- [ ] **TrackIR regression tests added** to [src/tests/](../src/tests/) covering: 3-blob input is unchanged; pose path is untouched by gaze fields.
- [ ] **Branch decision:** is this work landing on `master` (1.5.x line) or a feature branch? Internal-IPC changes mean a coordinated release; pick a target version up front.
- [ ] **AppImage delivery acknowledged** as the supported channel for gaze, given `HAVE_ONNXRUNTIME=OFF` in most source builds.

---

## 5. Out of scope

- WiFi-CSI body tracking (RuView). Documented separately in [ruview_integration_assessment.md](ruview_integration_assessment.md) — recommendation: do not pursue.
- Replacing the existing TrackIR or webcam pipelines. This roadmap *extends*, never replaces.
- Foveated rendering implementation in games. We provide gaze; games render.

---

## 6. Open questions

These don't block Phase 0/1 but need answers before Phase 3:

1. Does Tobii's Stream Engine API have signature/auth checks that prevent third-party impersonation? (Affects Phase 0 option B feasibility.)
2. Does Eyeware Beam iOS emit a documented UDP format, or does it require their relay app? (Affects Phase 1 — if Beam's protocol is undocumented, OpenTrack-only is the realistic input target.)
3. What is the actual latency of the existing two-stage pipeline on representative hardware? Without this baseline, the "third stage will cost X ms" claim is unverifiable.
4. Should gaze be per-profile or global? (Affects `ltr_gui` and `CMD_PROFILE_CHANGE` IPC handling — see [CLAUDE.md](../CLAUDE.md) IPC rules.)
