# Integration Assessment: Eyeware Beam & Native Gaze Tracking

**Date:** 2026-03-25
**Subject:** Technical roadmap for gaze estimation in Linuxtrack.

## Executive Summary

Linuxtrack can achieve **feature parity with Eyeware Beam** through native ONNX model integration, not external wrappers. The infrastructure already exists.

---

## 1. Current Architecture

Linuxtrack's neural network pipeline (`neuralnet_tracker.cpp`):

```text
┌──────────────────┐    ┌──────────────────┐
│ head-localizer   │ →  │ head-pose        │ → [Pitch, Yaw, Roll, X, Y, Z]
│ .onnx            │    │ .onnx            │
└──────────────────┘    └──────────────────┘
```

**Existing capabilities:**
- ONNX Runtime integration (CPU/GPU)
- OpenCV image preprocessing
- One Euro filter for smoothing
- Multi-model inference pipeline

---

## 2. Gaze Estimation: Academic Foundations

The "proprietary AI" in commercial trackers is based on published research:

| Model | Paper | Accuracy | Notes |
| :--- | :--- | :--- | :--- |
| **L2CS-Net** | [arXiv:2203.03339](https://arxiv.org/abs/2203.03339) | 3.92° MPIIGaze | State-of-the-art, two-branch CNN |
| **MobileGaze** | [GitHub](https://github.com/yakhyo/gaze-estimation) | Lightweight | **Pretrained ONNX available** |
| **ETH-XGaze** | [ETH Zurich](https://ait.ethz.ch/xgaze) | Dataset | 1M+ images, extreme poses |
| **GazeTR** | [ICPR 2022](https://github.com/yihuacheng/GazeTR) | Transformer | High accuracy, heavier |

---

## 3. Implementation Roadmap

### Phase 1: Native Gaze Model Integration

**Target:** Add gaze estimation to existing ONNX pipeline.

```text
Extended Pipeline:
┌──────────────────┐    ┌──────────────────┐    ┌──────────────────┐
│ head-localizer   │ →  │ head-pose        │ →  │ gaze-estimator   │
│ .onnx            │    │ .onnx            │    │ .onnx            │
└──────────────────┘    └──────────────────┘    └──────────────────┘
                                                 (L2CS-Net / MobileGaze)
                                                        ↓
                                               [GazeYaw, GazePitch]
```

**Tasks:**
1. Download/convert MobileGaze or L2CS-Net to ONNX
2. Extend `neuralnet_tracker.cpp` with third inference stage
3. Add gaze output fields to pose structure
4. Calibration UI in ltr_gui (gaze vector → screen coordinates)

**Difficulty:** Medium
**Dependencies:** None (ONNX Runtime already integrated)

### Phase 2: UDP Network Input Driver

**Target:** Accept pose data from external sources (Eyeware Beam iOS, OpenTrack, etc.)

```text
┌─────────────────────┐
│ External Source     │
│ (Beam iOS/OpenTrack)│
└──────────┬──────────┘
           │ UDP :4242 (48 bytes: 6 doubles)
           ▼
┌─────────────────────┐
│ libnet_input.so     │  ← New driver plugin
│ - UDP listener      │
│ - Decodes packet    │
│ - cal.h interface   │
└──────────┬──────────┘
           ↓
┌─────────────────────┐
│ ltr_server1         │
└─────────────────────┘
```

**Tasks:**
1. New source file: `src/net_input_driver.c`
2. Implement device abstraction (`cal.h` interface)
3. Add "Network Input" device type to GUI
4. Port/protocol configuration in preferences

**Difficulty:** Medium
**Dependencies:** None (standard POSIX sockets)

---

## 4. What This Replaces

| Old Approach | Problem | New Approach |
| :--- | :--- | :--- |
| Python relay wrapper | External dependency, latency | Native UDP driver |
| External gaze software | Proprietary, paid, Windows | Native ONNX gaze model |
| Eyeware SDK | Windows-only, closed | Academic models (L2CS-Net) |

---

## 5. Hardware Comparison

| Feature | Tobii 5 | Eyeware Beam | Linuxtrack (Planned) |
| :--- | :--- | :--- | :--- |
| Sensor | Dedicated IR | iPhone TrueDepth / Webcam | Webcam |
| Detection | PCCR (corneal glint) | Neural net | Neural net (L2CS-Net) |
| Calibration | Factory | Per-session | Per-session |
| Platform | Windows only | Windows + iOS | **Linux, macOS, Windows** |
| Cost | ~$250 | ~$30 | **Free** |

---

## 6. Conclusion

Linuxtrack should pursue **native gaze estimation** using published academic models, not external wrappers. The ONNX infrastructure exists; this is an extension, not a rewrite.

**Priority order:**
1. Native gaze model (L2CS-Net/MobileGaze) - differentiator
2. UDP input driver - interoperability with external sources
