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

- [x] Audit AppImage contents, strip unused Qt plugins (Added exclusion list)
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

## Post-Mortem: One Euro Filter Timing Fix BROKE TRACKING (2026-01-08)

**Date:** 2026-01-08  
**Severity:** CRITICAL (Complete tracking failure)  
**Status:** REVERTED - Awaiting Team Review

### The Problem

User reported One Euro filter settings (smoothing) not taking effect in X-Plane. Investigation found:

- Settings ARE saved correctly in config
- Filter IS being called in `axis.c`
- Hardcoded `dt = 1.0f / 120.0f` assumes 120Hz, but TrackIR runs ~60Hz

### What Was Attempted

```c
// axis.c - BROKEN CODE (reverted)
static struct timespec last_ts = {0, 0};
struct timespec now;
clock_gettime(CLOCK_MONOTONIC, &now);
float dt = (now.tv_sec - last_ts.tv_sec) + 
           (now.tv_nsec - last_ts.tv_nsec) / 1e9f;
last_ts = now;
```

### What Broke

After installing:

- Clicking "Start" in ltr_gui → **nothing happens**
- 3D View window **no longer moves**
- Complete tracking failure

### Root Cause (Suspected)

> ⚠️ **TEAM REVIEW NEEDED** - We don't fully understand why this broke.

Possible issues:

1. **Static variable across axes?** Each axis should have its own `last_ts` but we used a single `static` variable
2. **First-frame timing?** The first call returns a bogus dt before `last_ts` is initialized
3. **Thread safety?** `ltr_int_filter_axis` is called from multiple threads with a shared static
4. **Clock overhead?** `clock_gettime` syscall in hot path?

### Lessons Learned

1. **Protocol violation:** Changes were implemented without team review
2. **Hot path modification:** Filter code is in critical tracking loop
3. **Insufficient testing:** Should have tested with `ltr_pipe` before installing

### Team Input Requested

| Question | Suggested Reviewer |
|----------|-------------------|
| Is per-axis static timing needed? | **LINUS** |
| Is `clock_gettime` too slow for hot path? | **CARMACK** |
| Should dt be passed from runloop instead? | **JAANA** |
| Alternative: use frame counter delta? | **ALL** |

### Next Steps (BLOCKED ON REVIEW)

1. Keep original hardcoded 120Hz for now
2. Get team consensus on correct approach
3. Implement with ONE testable change at a time
4. Test in isolation before system install

### Team Review Responses (2026-01-08)

#### [LINUS TORVALDS] - C Systems

> **Verdict: The fix was fundamentally broken.**

The problem is obvious once you look at it:

```c
// BROKEN: Single static shared across ALL axes
static struct timespec last_ts = {0, 0};  // <-- This is the bug
```

Each of the 6 axes (PITCH, ROLL, YAW, TX, TY, TZ) calls `ltr_int_filter_axis`. With a single static `last_ts`, the **first axis** gets a correct `dt`, but axes 2-6 get `dt ≈ 0` because they're called in rapid succession within the same frame.

When `dt → 0`, the One Euro filter math breaks. The cutoff frequency goes to infinity and the filter outputs garbage or NaN.

**The fix needs per-axis timing state**, which already exists: `axis->one_euro_state` should include its own `last_timestamp`.

---

#### [JOHN CARMACK] - Real-time

> **Verdict: Wrong approach. Don't calculate dt in the filter.**

`clock_gettime` in a per-axis filter is wrong for two reasons:

1. **6 syscalls per frame** (one per axis) - adds ~3-6µs overhead each
2. **Doesn't reflect actual data age** - we want time since *last USB frame*, not time since *last filter call*

The **correct pattern** is to calculate dt ONCE at the frame level, in `tracking.c` where we have the USB timestamp, and pass it down:

```c
// tracking.c - calculate dt once per USB frame
static struct timespec last_frame_ts;
float dt = calculate_frame_dt(&last_frame_ts);

// Pass dt to filter
pose->pitch = ltr_int_filter_axis(axes, PITCH, raw, &filtered, dt);
```

This is how every serious real-time filter works. The filter doesn't own its timing - the caller does.

---

#### [JAANA DOGAN] - Systems/IPC

> **Verdict: Architecture issue. The filter should not know about time.**

Looking at the data flow:

```
USB Frame → tracking.c → ltr_int_filter_axis() → pose output
```

The filter is currently "pull-based" for timing - it tries to figure out when it was last called. This is fragile.

**Recommended approach:**

1. Add `float dt` parameter to `ltr_int_filter_axis()`
2. Calculate dt in `tracking.c` using USB frame counter or monotonic clock
3. `one_euro_filter()` already takes dt - just pass it through

This is a 3-line change to the function signature plus a small change in the caller. No statics, no timing code in the filter.

---

### Team Consensus

| Approach | Linus | Carmack | Jaana |
|----------|-------|---------|-------|
| Per-axis static timing | ❌ "Still wrong" | ❌ "Too many syscalls" | ❌ "Fragile" |
| Pass dt from caller | ✅ "Clean" | ✅ "Correct" | ✅ "Architectural" |
| Use frame counter delta | ⚠️ "Needs USB timing" | ✅ "Preferred" | ✅ "Observable" |

**Approved Approach:**

1. Modify `ltr_int_filter_axis()` signature to accept `float dt`
2. Calculate dt in `tracking.c` from USB frame timing
3. Pass dt through the call chain
4. Test with `ltr_pipe` before system install

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
| 2026-01-06 | AppImage Bloat ("Hoarding") | Arch builds pull massive recursive dependencies (VTK, OpenCV-Viz) | ✅ `make_appimage.sh` exclusion list |
| 2026-01-07 | Cross-distro incompatibility | GLIBC versions from bleeding-edge hosts break AppImages | ✅ `Dockerfile.build` for hermetic base |
| 2026-01-08 | **axis.c timing fix broke tracking** | NEVER modify hot path code without team review. Static variables in filter = disaster. | ✅ Team-reviewed fix deployed - awaiting user confirmation |

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
