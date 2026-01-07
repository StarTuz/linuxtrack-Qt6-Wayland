# Linuxtrack Team Review: Consolidated Feedback

**Date:** 2026-01-06  
**Version Reviewed:** v1.1.9

---

## Executive Summary

| Team | Overall Grade | Top Priority |
|------|---------------|--------------|
| **Red Team** | B+ | IPC message authentication, profiling |
| **Blue Team** | B+ | Compiler warnings, first-run UX |

---

## Red Team Feedback (Review/Audit)

### [MITCHELL HASHIMOTO] Infrastructure

**Grade:** A-

| ✅ Strengths | ⚠️ Concerns |
|-------------|-------------|
| CMake migration complete | AppImage size (169MB) |
| $ORIGIN RPATH | 32-bit Wine multi-pass build complexity |
| GitHub Actions CI | No dependency version lockfile |

**Action Items:**

- [ ] Audit AppImage contents, strip unused Qt plugins
- [ ] Create `DEPENDENCIES.md` with tested version matrix
- [ ] Consider Flatpak as additional distribution target

---

### [ALEX STAMOS] Security

**Grade:** B+

| ✅ Strengths | ⚠️ Concerns |
|-------------|-------------|
| sprintf → snprintf migration | Wine bridge IPC lacks message auth |
| strtok → strtok_r | USB firmware not hash-verified |
| GUARDRAILS.md in place | Hotkey daemon lacks rate limiting |

**Action Items:**

- [ ] Add message authentication to shared memory IPC
- [ ] Implement firmware hash verification (TIRViews.dll SHA256)
- [ ] Rate-limit hotkey commands
- [ ] Add `/tmp/ltr_mmap2` to No-Touch zones

---

### [JAANA DOGAN] Systems/IPC

**Grade:** B+

| ✅ Strengths | ⚠️ Concerns |
|-------------|-------------|
| Clean daemon architecture | One Euro Filter in hot path |
| Live profile switching | Mutex contention unknown |
| Shared memory IPC | No latency metrics |

**Action Items:**

- [ ] Profile tracking hot path with timestamps
- [ ] Add latency metrics to `aiam_check.sh`
- [ ] Consider lock-free structures for pose data
- [ ] Verify frame counter check before reprocessing

---

### [ANDREJ KARPATHY] ML/CV (On-Call)

**Grade:** A (No action needed)

**Verdict:** One Euro Filter is correct choice. No ML improvements needed unless users report specific facetracker failures.

---

## Blue Team Feedback (Build/Ship)

### [LINUS TORVALDS] C Systems

**Grade:** B+

| ✅ Strengths | ⚠️ Concerns |
|-------------|-------------|
| Buffer safety (snprintf) | Mixed C/C++ in server code |
| Thread-safe APIs | USB unplug handling unclear |
| Clean driver separation | Magic numbers in tracking code |

**Action Items:**

- [ ] Add `-Wall -Wextra -Werror` to CMake
- [ ] Document memory ownership in headers
- [ ] Verify all error paths free resources
- [ ] Replace magic numbers with named constants

---

### [JOHN CARMACK] Real-time

**Grade:** A-

| ✅ Strengths | ⚠️ Concerns |
|-------------|-------------|
| Early exit patterns | No profiling infrastructure |
| State machine in plugin | Lock duration unknown |
| Reconnection logic | Filter efficiency unclear |

**Target Latencies:**

| Stage | Target |
|-------|--------|
| USB read | <5ms |
| Filter application | <100µs |
| IPC round-trip | <500µs |
| End-to-end | <10ms |

**Action Items:**

- [ ] Add timestamp-based profiling mode
- [ ] Measure mutex hold time in `ltr_srv_master.cpp`
- [ ] Verify One Euro runs only on new data
- [ ] Create `--profile` flag for debug builds

---

### [SARAH DRASNER] UI/UX

**Grade:** B

| ✅ Strengths | ⚠️ Concerns |
|-------------|-------------|
| Qt6 compatibility | No first-run wizard |
| Real-time sliders | Technical error messages |
| Profile system | Unclear visual hierarchy |

**Quick Wins:**

| Current | Improved |
|---------|----------|
| "OK" button | "Save Profile" |
| "Firmware not found" | "Click here to install firmware" |
| Silent toggles | Toast notifications |

**Action Items:**

- [ ] Improve error messages to be actionable
- [ ] Add status indicator (green/yellow/red)
- [ ] Add tooltips with keyboard shortcuts
- [ ] Add "What's this?" for One Euro parameters

---

## Priority Matrix

| Priority | Action Item | Owner | Effort |
|----------|-------------|-------|--------|
| P1 | Add `-Wall -Wextra` to CMake | Linus | Low |
| P1 | Add profiling timestamps | Carmack | Medium |
| P1 | Improve error messages | Sarah | Low |
| P2 | Profile tracking hot path | Jaana | Medium |
| P2 | Audit AppImage size | Mitchell | Medium |
| P2 | Add rate limiting to hotkeys | Alex | Low |
| P3 | Firmware hash verification | Alex | Medium |
| P3 | First-run wizard | Sarah | High |
| P3 | Document memory ownership | Linus | Low |

---

## Post-Mortem: v1.1.6-v1.1.9 X-Plane View Regression

**Date:** 2026-01-06  
**Severity:** High  
**Reverted to:** v1.1.4 (commit b9e948f)

### What Happened

| Version | Change | Result |
|---------|--------|--------|
| v1.1.4 | Robustness/retry logic added | ✅ Working |
| v1.1.6 | "fwfa behavior" - suspend on view change | ❌ Broke Shift-4 |
| v1.1.7-v1.1.9 | Band-aid fixes on broken logic | ❌ Still broken |

### Root Cause

v1.1.6 added untested code that called IPC in the X-Plane flight loop:

```c
if (view_changed && was_in_cockpit) {
  linuxtrack_suspend();  // IPC call - breaks view switching
}
```

This code **never existed before v1.1.6** and was based on a misunderstanding of the fwfa fork.

### Team Analysis

**[CARMACK]:** *"The original v1.1.4 code handled view switching correctly with just early return. No IPC needed."*

**[LINUS]:** *"The fwfa fork was *simpler* than our code, not more complex. We added complexity that wasn't needed."*

### Lessons Learned

1. **No IPC in flight loop callbacks** - must be fast
2. **Test X-Plane view switching** before merging plugin changes
3. **Require Carmack sign-off** on real-time plugin changes

### Prevention

Added to GUARDRAILS.md:

- X-Plane plugin changes require actual X-Plane testing
- IPC calls forbidden in flight loop without justification

---

## Self-Evolving Capability Audit (SECA)

> *"Track what we learn, not just what we build."* — Team Consensus

### Purpose

SECA captures capability gaps discovered during incidents and tracks how the process evolves in response. This prevents the same mistakes from recurring and calibrates team confidence over time.

### Capability Evolution Log

| Date | Incident | Lesson Learned | Guardrail Updated |
|------|----------|----------------|-------------------|
| 2026-01-06 | X-Plane view regression (v1.1.6-v1.1.9) | Don't make multiple untested changes to T2 files | ✅ GUARDRAILS.md X-Plane section |
| 2026-01-06 | Debugging by mutation | Make ONE change, test, iterate - never multiple changes without testing | ✅ Added to this log |
| 2026-01-06 | Incomplete fwfa fork comparison | Side-by-side diff required before porting fixes from forks | 🔄 Pending |
| 2026-01-06 | Misunderstanding of "fwfa behavior" | fwfa fork was SIMPLER, not more complex - verify assumptions | ✅ Post-mortem documented |

### Confidence Calibration

| Prediction | Outcome | Calibration |
|------------|---------|-------------|
| "Remove suspend/wakeup will fix view" | ❌ No change | Overconfident - IPC wasn't the only issue |
| "Rate-limiting will help" | ❌ Made it worse | Wrong hypothesis |
| "v1.1.4 was the working state" | ❌ Issue predates v1.1.4 | Incomplete git archaeology |

### Next Audit Triggers

- After any regression is discovered
- After any debugging session exceeds 30 minutes
- When reverting changes
- Monthly: Review log for patterns
