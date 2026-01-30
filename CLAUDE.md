# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Linuxtrack is a head-tracking solution for Linux that enables applications (primarily flight simulators and games) to respond to head movement. It supports TrackIR USB devices, webcams, and OpenCV face tracking. Windows games are supported through a Wine/Proton bridge.

## Build Commands

```bash
# Standard build
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)

# Install (default: /opt/linuxtrack)
sudo cmake --install .

# Run from build directory (no install needed)
./build/src/ltr_gui
```

### Validation & Testing

```bash
# Run unit tests
cd build && ctest --output-on-failure

# Validate AppImage (after building)
./scripts/validate_appimage.sh Linuxtrack-x86_64.AppImage

# Multi-distro smoke test (requires Docker)
./scripts/smoke_test.sh

# Check RPATH on binaries
readelf -d build/src/ltr_server1 | grep -E "(RPATH|RUNPATH)"
```

### Wine Bridge (32-bit + 64-bit)

The Wine bridge requires both 32-bit and 64-bit components. CMake handles this automatically if Wine is available:

```bash
# CMake auto-detects Wine and builds both architectures
cmake --build build -j$(nproc)

# If 32-bit libs are missing (common on Arch), only 64-bit builds
# Install lib32-glibc lib32-gcc-libs for 32-bit support
```

### Dependencies (Arch Linux)

```bash
# Core
sudo pacman -S base-devel cmake ninja pkg-config mxml libusb zlib nlohmann-json

# Qt6
sudo pacman -S qt6-base qt6-tools qt6-5compat qt6-help

# Video/Tracking
sudo pacman -S v4l-utils opencv

# Wine (for Windows game bridge)
sudo pacman -S wine wine-mono wine-gecko

# Optional
sudo pacman -S liblo      # OSC support
sudo pacman -S libcwiid   # Wiimote (AUR)
```

## Architecture

```
Applications (X-Plane, Wine games, OSC clients)
                    ↓
          liblinuxtrack.so (Client API - src/ltlib.c)
                    ↓
           ltr_server1 (Daemon - src/ltr_server1.c)
                    ↓
             libltr.so (Core Engine - src/tracking.c, axis.c, pose.c)
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
| `xlinuxtrack9.so` | `src/xlinuxtrack9.c` | X-Plane 9+ plugin (64-bit only) |
| Wine bridge | `src/wine_bridge/` | NPClient.dll, FreeTrack emulation |
| `ltr_udp` | `src/` | OpenTrack UDP bridge (port 4242) |
| `ltr_hotkeyd` | `src/` | Native Linux global hotkey daemon |

### IPC

- **Shared memory**: Primary client-server communication
- **Pipes**: Alternative interface (`ltr_pipe`)
- **UDP bridge**: Port 4242 (OpenTrack protocol), Port 4243 (hotkey commands)

### Preferences

Uses mINI library for INI file parsing. Config files stored in `~/.config/linuxtrack/`.

## Critical Rules

### X-Plane Plugin (`xlinuxtrack9.c`)

> **CAUTION:** v1.1.6-v1.1.9 had regressions from untested plugin changes.

- Changes MUST be tested with actual X-Plane before merge
- Test external view switching (Shift-4) with TrackIR active
- **FORBIDDEN:** IPC calls (`linuxtrack_suspend`, `linuxtrack_wakeup`, `linuxtrack_get_tracking_state`) in flight loop callback without careful review
- Keep simple early-return pattern for non-cockpit views

### Wine Bridge

- Test both 32-bit AND 64-bit DLLs when modifying
- Never hardcode `WINEARCH` - let prefix determine architecture
- Surgical injection copies DLLs directly, no NSIS installer needed
- IDE lint errors are false positives (winegcc include paths not recognized)

### Protected Paths (Never Modify Without Explicit Request)

- `~/.config/linuxtrack/` - User configuration
- `~/.config/linuxtrack/tir_firmware/` - TrackIR firmware files
- Wine prefixes during surgical injection

### Build System

- Use CMake (not Autotools) for all changes
- Maintain Qt6 compatibility
- Uses `$ORIGIN` RPATH for relocatable binaries
- C++17 required (`std::filesystem` in preference system)

## Key Documentation

- `HANDOFF.md` - Detailed technical notes, version history, and post-mortems
- `GUARDRAILS.md` - Non-negotiable requirements for code changes
- `README.md` - User-facing setup and troubleshooting
- `MODERNIZATION_ROADMAP.md` - Completed and planned improvements

## Current Version

v1.3.1 - Qt6/Wayland compatible, OpenGL ES 3.0 shaders, One Euro filter, UDP bridge, 32-bit auto-detection
