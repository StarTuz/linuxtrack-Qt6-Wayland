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

### Wine Bridge (32-bit + 64-bit)

The Wine bridge requires a two-pass build for both architectures. The CI workflow handles this automatically, but for manual builds:

```bash
# Pass 1: 32-bit Wine components
cd src/wine_bridge && make WINEARCH=win32

# Pass 2: 64-bit Wine components
cd src/wine_bridge && make WINEARCH=win64
```

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

### IPC

- Shared memory for client-server communication
- Pipes as alternative interface
- UDP bridge (port 4242) for OpenTrack protocol

### Preferences

Uses mINI library for INI file parsing. Config files stored in `~/.config/linuxtrack/`.

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

### Wine Bridge

- Test both 32-bit AND 64-bit DLLs when modifying
- Never hardcode `WINEARCH` - let prefix determine architecture
- Surgical injection copies DLLs directly, no NSIS installer needed

## Key Documentation

- `HANDOFF.md` - Detailed technical implementation notes and version history
- `GUARDRAILS.md` - Non-negotiable requirements for code changes
- `README.md` - User-facing setup and troubleshooting guide
- `MODERNIZATION_ROADMAP.md` - Future improvements

## Current Version

v1.1.12 - Qt6/Wayland compatible, OpenGL ES 3.0 shaders, One Euro filter, UDP bridge
