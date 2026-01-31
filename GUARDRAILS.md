# Guardrails: Linuxtrack

> Note for Agents: These are NON-NEGOTIABLE requirements. Code that violates these MUST NOT be merged.

## 1. AIAM: Agent Governance

### No-Touch Zones

- **FORBIDDEN:** Modifying or deleting system-level binaries or configuration outside the workspace.
- **FORBIDDEN:** Deleting project source files based on heuristic assumptions without Explicit Verification (EV).
- **FORBIDDEN:** Running `git checkout`, `git reset`, `git restore` without explicit user permission (local fixes are uncommitted).
- **PROTECTED:** Wine bridge files during surgical injection operations.
- **PROTECTED:** TrackIR firmware extraction paths (`~/.config/linuxtrack/tir_firmware/`).
- **PROTECTED:** User config files (`~/.config/linuxtrack/`) - read to understand, never modify without explicit request.

### Action Risk Tiers

- **Tier 0 (Safe):** Read-only, linting, UI state, viewing documentation.
- **Tier 1 (Normal):** Incremental code edits, new feature files, documentation updates.
- **Tier 2 (High-Risk):** CMake dependency changes, Wine bridge modifications, X-Plane plugin changes (`xlinuxtrack9.c`).
- **Tier 3 (Restricted):** DELETIONS, binary changes, Wine DLL rebuilds, global system environment changes.

### Mandatory Verification (EV)

- **REQUIRED:** Before T2/T3 actions, agents MUST use `view_file` or `ls` to provide state proof to the user.
- **REQUIRED:** All T3 actions must be logged with a `justification` in the project audit log.
- **REQUIRED:** Run `scripts/aiam_check.sh` before major system-affecting changes.

---

## 2. Universal Standards

### Input Validation

- **Confidence thresholding:** Reject ambiguous inputs.
- **Rate Limiting:** Prevent command flooding.

### Output Integrity

- **Entity Verification:** Ensure objects exist before acting.
- **High-Risk Confirmation:** Require "Say confirm to proceed" for dangerous commands.

### Error Handling

- **No Silent Failures:** Every error path must be logged or handled.
- **Trace-to-Fix:** Focus on execution flow, not environment assumptions.

### Audit Logging

- **Decision Tracking:** Log all commands with source, confidence, and action status.

---

## 3. Linuxtrack-Specific Rules

### Critical Workflow File

- **Always read** `.agent/workflows/critical-rules.md` before starting work.

### Build System

- Use CMake (not Autotools) for all new build changes.
- Maintain Qt6 compatibility with Qt5 fallback pattern.

### Wine Bridge

- Test 32-bit AND 64-bit DLLs when modifying Wine bridge code.
- Never assume `WINEARCH` - let prefix determine architecture.

### X-Plane Plugin

> [!CAUTION]
> v1.1.6-v1.1.9 suffered a regression from untested X-Plane plugin changes. See TEAM_FEEDBACK.md post-mortem.

- **REQUIRED:** Changes to `xlinuxtrack9.c` MUST be tested with actual X-Plane before merge.
- **REQUIRED:** Test external view switching (Shift-4) with TrackIR active.
- **FORBIDDEN:** IPC calls (`linuxtrack_suspend`, `linuxtrack_wakeup`, `linuxtrack_get_tracking_state`) in flight loop callback without Carmack review.
- **KEEP:** Simple early-return pattern for non-cockpit views.
- Maintain reconnection/retry logic for server disconnects (but rate-limit if in hot path).

### 3D Rendering & Transparency

- **FORBIDDEN:** Removing the alpha-mask reset block at the end of `GLWidget::paintGL`.
- **REQUIRED:** `QSurfaceFormat` in `main.cpp` must keep `AlphaBufferSize(0)` to prevent compositor bleed-through.
- **REQUIRED:** Maintain `Qt::WA_NoSystemBackground` on `GLWidget` to ensure proper opaque compositing.
