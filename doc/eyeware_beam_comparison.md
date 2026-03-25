# Competitive Analysis: Eyeware Beam vs. Linuxtrack

**Date:** 2026-03-25
**Subject:** Technical comparison of head and eye tracking capabilities.

## Overview

This document compares **Eyeware Beam** (a commercial Windows/iOS solution) with the current **Linuxtrack** project and **Tobii Eye Tracker 5** to evaluate technological overlap and differentiation.

## Technical Comparison

| Feature | Eyeware Beam | Linuxtrack (Current) | Linuxtrack (Planned) | Tobii Eye Tracker 5 |
| :--- | :--- | :--- | :--- | :--- |
| **Logic** | Proprietary AI (ONNX) | **Open-source (ONNX)** | **Open-source (ONNX)** | Proprietary (Closed) |
| **Filtering** | One Euro Filter | **One Euro Filter** | **One Euro Filter** | Proprietary |
| **Head Tracking** | 6DoF (AI-based) | **6DoF (IR & AI)** | **6DoF (IR & AI)** | 6DoF (IR-based) |
| **Eye/Gaze Tracking** | Yes (Gaze) | No (Head only) | **Yes (L2CS-Net/MobileGaze)** | Yes (Gaze) |
| **Platform** | Windows (+ iOS) | **Linux & macOS** | **Linux & macOS** | Windows Only |
| **Linux Gaming** | Viable (iOS/UDP) | **Native (Best)** | **Native (Best)** | No (Unsupported) |
| **Cost** | Paid (~$30) | **Free** | **Free** | Paid (~$250) |

## De-mystifying the "AI" Marketing

Commercial solutions like Eyeware Beam and Tobii market "proprietary AI" but the underlying technology is based on **publicly available academic research**.

### 1. Robust Filtering (One Euro Filter)

Linuxtrack already utilizes the **One Euro filter** (`src/one_euro_filter.c`). This is the same industry-standard algorithm used by commercial trackers, providing low-latency response with high-frequency jitter reduction.

### 2. Neural Network Head Tracking (ONNX)

Linuxtrack's `neuralnet_tracker.cpp` implements a two-stage pipeline using **ONNX Runtime**:

* **Stage 1 (Localizer):** `head-localizer.onnx` - face detection
* **Stage 2 (Pose Estimator):** `head-pose-0.4-*.onnx` - pitch, yaw, roll estimation

### 3. Gaze Estimation: The Missing Piece (Solvable)

Gaze tracking is **not proprietary magic**. The academic foundations are published:

| Model | Year | Accuracy | Source | ONNX Ready |
|-------|------|----------|--------|------------|
| **L2CS-Net** | 2022 | 3.92° (MPIIGaze) | [arXiv:2203.03339](https://arxiv.org/abs/2203.03339) | Yes |
| **MobileGaze** | 2024 | Lightweight/Mobile | [GitHub](https://github.com/yakhyo/gaze-estimation) | **Pretrained** |
| **ETH-XGaze** | 2020 | Dataset baseline | [ETH Zurich](https://ait.ethz.ch/xgaze) | Yes |

**Implementation path:** Add a third ONNX model to the existing pipeline:

```
Current:  [localizer] → [head-pose] → Pitch/Yaw/Roll
Extended: [localizer] → [head-pose] → [gaze-estimator] → Pitch/Yaw/Roll + GazeX/GazeY
```

The ONNX Runtime infrastructure already exists in Linuxtrack. Adding gaze estimation is a matter of integrating a pretrained model (MobileGaze or L2CS-Net) and extending the output structure.

## What Tobii Has That Webcams Don't

| Feature | Tobii 5 (Hardware) | Webcam-based (Software) |
|---------|-------------------|-------------------------|
| Dedicated IR sensor | 850nm NIR illumination | RGB only |
| Corneal reflection (PCCR) | Hardware glint detection | Pupil-only (neural net compensates) |
| Calibration-free | Factory calibrated | User calibration required |

Tobii's advantage is **hardware** (dedicated IR + corneal reflection), not algorithms. Webcam-based solutions like L2CS-Net achieve competitive accuracy through better neural networks.

## Conclusion

Linuxtrack's head tracking is already on par with commercial solutions. **Gaze estimation can be added natively** using published academic models (L2CS-Net, MobileGaze) without external wrappers or dependencies.

The goal is **feature parity with Eyeware Beam** through native ONNX integration, not reliance on external services.
