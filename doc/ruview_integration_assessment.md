# Integration Assessment: RuView WiFi-based Tracking

**Date:** 2026-03-25
**Subject:** Feasibility analysis for RuView → Linuxtrack integration.

## Executive Summary

**RuView is NOT suitable for precision head tracking in flight simulators or games.**

While technically interesting, RuView solves a different problem (room-scale body pose) than what Linuxtrack needs (precise 6DoF head orientation).

---

## What RuView Actually Is

[RuView](https://github.com/ruvnet/RuView) uses WiFi Channel State Information (CSI) to detect human presence and estimate body pose without cameras.

### Output Format

- **17 COCO body keypoints** (shoulders, elbows, wrists, hips, knees, ankles, nose, eyes, ears)
- **Room-space 3D coordinates** (X, Y, Z in meters relative to WiFi nodes)
- **NOT orientation data** (no pitch, yaw, roll)

### Hardware Requirements

| Configuration | Capability |
| :--- | :--- |
| Consumer WiFi only | Presence detection only (no pose) |
| ESP32-S3 mesh (3-6 nodes, ~$8 each) | Full 17-keypoint pose |
| Research NICs (Intel 5300, Atheros AR9580) | Full pose (Linux only) |

**Critical limitation:** Standard consumer WiFi routers cannot extract CSI data. Specialized hardware is required.

### Accuracy & Latency

| Metric | Claimed | Reality for Gaming |
| :--- | :--- | :--- |
| Presence detection | >95% accuracy | Irrelevant for tracking |
| Pose keypoint accuracy | "Sub-inch" with 6-node mesh | Untested for head-only |
| Latency | <100ms | Too slow for 120Hz flight sim |
| Head orientation | Not provided | **Fundamental gap** |

---

## Why It Doesn't Fit Linuxtrack

### The Fundamental Mismatch

| Linuxtrack Needs | RuView Provides |
| :--- | :--- |
| Head pitch (degrees) | Nose keypoint Y position (meters) |
| Head yaw (degrees) | Nose keypoint X position (meters) |
| Head roll (degrees) | **Nothing** |
| Sub-degree precision | "Sub-inch" body position |
| 60-120Hz update rate | 20-100Hz (claimed) |

**The problem:** RuView outputs "where is the head in the room" not "which direction is the head pointing." These are fundamentally different.

### Theoretical Derivation (Impractical)

You could *attempt* to derive head orientation from multiple keypoints:

```text
Yaw ≈ atan2(right_ear.x - left_ear.x, right_ear.y - left_ear.y)
Pitch ≈ angle between nose and ear midpoint
Roll ≈ angle of ear-to-ear line vs horizontal
```

**Problems:**

1. WiFi CSI resolution is insufficient for ear-level precision
2. Keypoint jitter would be extreme
3. No commercial WiFi provides the data anyway
4. The math assumes perfect keypoint detection

---

## Honest Assessment

### RuView's Actual Use Cases

- Smart home presence detection
- Healthcare/elderly fall detection
- Retail occupancy analytics
- Privacy-preserving surveillance alternative

### NOT Suitable For

- Precision head tracking for flight sims
- Any application requiring sub-degree orientation
- Consumer deployments (requires ESP32 mesh)

---

## Recommendation

**Do not pursue RuView integration for Linuxtrack.**

The technology is interesting for its intended purpose (privacy-preserving presence detection) but fundamentally cannot provide the precision head orientation data that flight simulators and games require.

If a user specifically wants to experiment with WiFi-based tracking, they can build their own relay. Linuxtrack should not invest development effort into this integration path.

### Alternative Focus

Development effort should go toward:

1. **Native gaze estimation** (L2CS-Net/MobileGaze) - real user value
2. **UDP input driver** (Eyeware Beam, OpenTrack interop) - actual demand
3. **TrackIR/Webcam refinement** - core functionality

---

## Sources

- [RuView GitHub](https://github.com/ruvnet/RuView)
- [RuView User Guide](https://github.com/ruvnet/RuView/blob/main/docs/user-guide.md)
- [WiFi DensePose Technical Deep Dive](https://deepwiki.com/ruvnet/RuView/4.3-pose-estimation-and-densepose)
