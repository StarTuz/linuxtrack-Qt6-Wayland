# Gaze Tracking Proposal — superseded

This document has been consolidated. The authoritative sources are:

- **[GAZE_TRACKING_ROADMAP.md](GAZE_TRACKING_ROADMAP.md)** — phasing, decisions, checklist.
- **[GAZE_TRACKING_ARCHITECTURE.md](GAZE_TRACKING_ARCHITECTURE.md)** — data flow, ABI rules, model options.

The original proposal and its review markup were merged into those two files on 2026-05-03. Reasons for consolidation:

- The original proposal's phase ordering was inverted (built inference and transport before confirming a destination consumer).
- The "4th blob" backward-compatibility claim was misleading — the wire format already carries 10 blobs, but overloading a positional blob slot for angular gaze is semantically wrong regardless. The roadmap mandates explicit `gaze_yaw` / `gaze_pitch` fields on `linuxtrack_full_pose_t` instead.
- The proposal collided with two parallel documents (`eyeware_beam_integration_assessment.md`, `eyeware_beam_comparison.md`) that told different stories. All three are now superseded.

If you arrived here from a stale link, follow the pointers above.
