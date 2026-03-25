# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Linuxtrack is a head-tracking solution for Linux that enables applications (primarily flight simulators and games) to respond to head movement. It supports TrackIR USB devices, webcams, and OpenCV face tracking. Windows games are supported through a Wine/Proton bridge.

**Current Version:** 1.4.0 (Wayland support and TrackIR responsiveness fixes)

## Build Commands

```bash
# Standard build
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)

# Install (default: /opt/linuxtrack)
sudo cmake --install .

# Run from build directory (no install needed)
./build/src/qt_gui/ltr_gui
```

### Build Individual Components

```bash
# GUI only (faster iteration)
cmake --build build --target ltr_gui

# Core library only
cmake --build build --target ltr

# Run unit tests
./build/src/lal/test_lal
```

### Wine Bridge

The Wine bridge builds automatically via CMake when `winegcc` is available. It builds both 32-bit and 64-bit components. CI handles this automatically.

To disable Wine bridge: `cmake .. -DBUILD_WINE_BRIDGE=OFF`

### Dependencies (Arch Linux)

Core: `base-devel cmake ninja pkg-config mxml libusb zlib`
Qt6: `qt6-base qt6-tools qt6-5compat qt6-help`
Video: `v4l-utils opencv`
Wine: `wine wine-mono wine-gecko`
Optional: `liblo` (OSC), `libcwiid` (Wiimote)

## Architecture

```
Applications (X-Plane, Wine games, OSC clients)
                    ↓
          liblinuxtrack.so (Client API)
                    ↓
           ltr_server1 (Daemon)
                    ↓
             libltr.so (Core Engine)
                    ↓
    Driver Plugins: libtir.so (TrackIR), libwc.so (Webcam), libft.so (Face)
                    ↓
              Hardware Layer
```

### Key Components

| Component | Location | Purpose |
|-----------|----------|---------|
| `ltr_gui` | `src/qt_gui/` | Qt6 configuration GUI |
| `ltr_server1` | `src/ltr_server1.c` | Background tracking daemon |
| `liblinuxtrack.so` | `src/ltlib.c` | Client library API |
| `libltr.so` | `src/tracking.c`, `axis.c`, `pose.c` | Core tracking engine |
| `xlinuxtrack9.so` | `src/xlinuxtrack9.c` | X-Plane 9+ plugin |
| Wine bridge | `src/wine_bridge/` | NPClient.dll, FreeTrack emulation |
| `ltr_udp` | `src/` | OpenTrack UDP bridge for native games |
| `ltr_hotkeyd` | `src/` | Global hotkey daemon (recenter/pause for native games) |
| `ltr_probe` | `src/` | Webcam probe/diagnostics utility |

### IPC

- Shared memory for client-server communication
- Pipes as alternative interface
- UDP bridge (port 4242) for OpenTrack protocol
- `CMD_PROFILE_CHANGE` message: broadcast by master to all slaves when profile switches; slaves reinitialize axes curves immediately. Any new IPC handler must process this message.

### Webcam Subsystem

The webcam driver (`src/webcam_driver.c`, `libwc.so`) uses V4L2 with libv4lconvert for pixel format negotiation. Key design points:
- Prefers native grayscale/YUV formats over compressed (MJPEG) for lower CPU overhead
- Multiple webcams are disambiguated by device identifier (not just index)
- Webcam preview mode is separate from tracking mask mode (`src/qt_gui/`)
- `ltr_probe` can enumerate V4L2 device capabilities for diagnostics

### Preferences

Uses mINI library for INI file parsing. Config files stored in `~/.config/linuxtrack/`.

## Git Workflow

**CRITICAL:** This repository contains uncommitted local modifications on top of upstream uglyDwarf/linuxtrack.

- **NEVER** run `git checkout`, `git reset`, `git restore`, or `git revert` without explicit user permission
- These commands will destroy the local fixes that make this project work on modern systems
- Always create new commits rather than amending when fixing issues

## Project-Specific Rules

### Protected Paths (Never Modify Without Explicit Request)

- `~/.config/linuxtrack/` - User configuration
- `~/.config/linuxtrack/tir_firmware/` - TrackIR firmware files
- Wine prefixes during surgical injection

### X-Plane Plugin Changes

**CAUTION:** v1.1.6-v1.1.9 had regressions from untested plugin changes.

- Changes to `xlinuxtrack9.c` MUST be tested with actual X-Plane
- Test external view switching (Shift-4) with TrackIR active
- Avoid IPC calls in the flight loop callback without careful review
- Keep simple early-return pattern for non-cockpit views

### Build System

- Use CMake (not Autotools) for all changes
- Maintain Qt6 compatibility
- Uses `$ORIGIN` RPATH for relocatable binaries
- CMake changes, Wine bridge modifications, and `xlinuxtrack9.c` edits are **Tier 2 (High-Risk)**; verify current file state before acting (per GUARDRAILS.md)

### AppImage / CI

- `xlinuxtrack9.so` requires the X-Plane SDK headers at `/usr/include/xplane_sdk/XPLM/`. The CI downloads XPSDK 4.3.0 automatically before the build step — without it, CMake silently skips building the plugin.
- `xlinuxtrack9.so` must only exist in `usr/lib/linuxtrack/` inside the AppImage. It is **not** a runtime dependency and must be excluded from the linuxdeploy `--library` loop (`packaging/appimage/make_appimage.sh`) to prevent a spurious duplicate copy in `usr/lib/`.

### 3D View Transparency (Qt6/GLES)

**CRITICAL:** The 3D view suffers from compositor "bleed-through" on modern Linux desktops (Wayland/X11). This is fixed via a multi-layered defense:

- **Surface Format (`main.cpp`):** `format.setAlphaBufferSize(0)` must be set before `QApplication` creation.
- **Widget Attributes (`glwidget.cpp`):** `setAttribute(Qt::WA_NoSystemBackground)` in constructor.
- **Clear Logic (`glwidget.cpp`):**
  - `glClearColor` must force alpha to `1.0f` in `initializeGL`.
  - **Alpha-Reset:** At the end of `paintGL`, the alpha channel MUST be explicitly cleared to `1.0f` using `glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE)` then `glClear(GL_COLOR_BUFFER_BIT)`.

### Wine Bridge

- Test both 32-bit AND 64-bit DLLs when modifying
- Never hardcode `WINEARCH` - let prefix determine architecture
- Surgical injection copies DLLs directly, no NSIS installer needed

## Key Documentation

- `HANDOFF.md` - Detailed technical implementation notes and version history (v1.0→v1.1.12)
- `GUARDRAILS.md` - Non-negotiable requirements; defines Action Risk Tiers (T0–T3)
- `README.md` - User-facing setup and troubleshooting guide
- `MODERNIZATION_ROADMAP.md` - Future improvements
- `.agent/workflows/critical-rules.md` - Read before starting any non-trivial work session

## Related Projects

There are related projects in the workspace - do NOT access without explicit permission:

| Project | Path | Status |
|---------|------|--------|
| linuxtrackfixed | `/home/startux/Code/linuxtrackfixed/` | **THIS PROJECT** - Active |
| tuxtracksold | `/home/startux/Code/tuxtracksold/` | Archived - Wine bridge never worked |
| tuxtracks | `/home/startux/Code/tuxtracks/` | New greenfield project (separate rules)
