# Linuxtrack Modernized (Proton & Qt6 Edition)

[![CI/CD Build](https://github.com/StarTuz/linuxtrack/actions/workflows/build-release.yml/badge.svg)](https://github.com/StarTuz/linuxtrack/actions)

This is a modernized fork of the [Linuxtrack](https://github.com/uglyDwarf/linuxtrack) project, updated to support modern Linux desktop standards (Qt6, Wayland, Modern OpenGL) and specifically optimized for **Proton/Steam Deck** environments.

## 🚀 Key Modernizations

Unlike other legacy forks, this version addresses deep technical debt to ensure long-term stability:

- **Native Firmware Manager (LAL)**: New "Manage Assets" GUI allows legal, drag-and-drop installation of proprietary TrackIR/SmartNav firmware without needing Wine or external tools.
- **Surgical Wine Injection**: No more fragile `.exe` installers! Directly inject bridge DLLs into Wine/Proton prefixes with one click.
- **Proton-Aware Environment**: Automatically configures `PROTON_NO_FSYNC` and `PROTON_NO_ESYNC` to prevent race conditions and tracking stutters in games like *Elite Dangerous*.
- **Qt6 & Wayland Support**: Fully ported from Qt4/5 to Qt6. Removed `QtX11Extras` to allow the GUI to run natively on Wayland.
- **Modern OpenGL (3.0+)**: Rewrote the 3D Tracking View using a programmable shader-based pipeline (GLES 3.0), replacing the broken 20-year-old fixed-function legacy code.
- **Bison/Flex Removal**: Replaced complex legacy configuration parsers with the lightweight, robust `mINI` library.
- **TrackIR 5 V3 Support**: Full hardware activation for the latest TrackIR 5 revisions.
- **Unified UDP Bridge**: New high-precision UDP stack that solves symmetry and range issues in full-screen games. Uses a unique coordinated architecture where hotkeys (Wine) trigger server-side (Linux) recentering on port 4243.

## 🎮 Verified Games & Apps

- ✅ **DCS World** (Proton, with Controller.exe hotkeys)
- ✅ **Elite Dangerous** (Proton 10/Steam Launcher, symmetric range via UDP)
- ✅ **Train Sim World 6** (Proton, full 6DOF via UDP Bridge)
- ✅ **X-Plane 12** (Native Linux Plugin)
- ✅ **X4 Foundations** (Via ltr_udp)

## 🔧 Hardware Setup (TrackIR/SmartNav)

### Udev Rules (Required for USB Access)

Your TrackIR/SmartNav device needs udev rules so Linux can access it without root privileges.

**Step 1: Copy the rules file**

```bash
# If installed to /opt/linuxtrack:
sudo cp /opt/linuxtrack/share/linuxtrack/99-TIR.rules /etc/udev/rules.d/

# If using the AppImage, extract it first:
./Linuxtrack-*.AppImage --appimage-extract
sudo cp squashfs-root/usr/share/linuxtrack/99-TIR.rules /etc/udev/rules.d/
```

**Step 2: Reload udev rules**

```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```

**Step 3: Unplug and replug your TrackIR device**

> [!TIP]
> If you still see "permissions problem" after following these steps, try logging out and back in, or rebooting.

## 🛠️ Installation

### 1. Build from Source (CMake)

**Prerequisites:** Qt6 (likely 6.10+ recommended), OpenGLES/OpenGL drivers, libusb-1.0, libmxml, liblo.

```bash
git clone https://github.com/StarTuz/linuxtrack.git
cd linuxtrack
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
```

### 2. AppImage Distribution

**Generic AppImage:** The generic CI build is currently experimental due to Qt6 ABI complexity.
**Arch Linux AppImage:** A verified, working AppImage (built against Arch system Qt) is available. This version uses the host system's Qt libraries for maximum compatibility on rolling release distros.

### 3. Configure Wine/Proton Support

1. Open `ltr_gui`.
2. Go to the **Misc.** tab.
3. Click **"Install Linuxtrack-Wine support..."**.
4. Select your game's Wine prefix from the auto-discovered list.
5. Controller.exe is automatically installed for Pause/Recenter hotkeys.

## 🗺️ Roadmap & Next Steps

Current progress is tracked in [MODERNIZATION_ROADMAP.md](MODERNIZATION_ROADMAP.md).

**Completed:**

- [x] Qt6 Migration & Wayland safety.
- [x] Modern OpenGL Shader-based 3D View.
- [x] Bison/Flex to mINI migration.
- [x] Surgical Wine/Proton Bridge Injection.
- [x] Project LAL (Native Firmware Manager) Phase 3 Integration.
- [x] Automatic Steam/Proton/Lutris/Bottles Prefix Discovery.
- [x] Controller.exe for customizable Pause/Recenter hotkeys.
- [x] Linux-native global hotkey daemon with **Wayland support** (via `xdg-desktop-portal` GlobalShortcuts).
- [x] Coordinated UDP Bridge for symmetric 6DOF tracking.

**Upcoming:**

- [ ] Native UI for model scaling/offsets.
- [-] AppImage/Flatpak distribution (Arch AppImage verified).

---

## ⌨️ Native Hotkeys (Wayland/X11)

The `ltr_hotkeyd` daemon provides global hotkeys for recenter and pause without needing alt-tab.

### Default Hotkeys

| Action | Key |
| --- | --- |
| Recenter Tracking | F12 |
| Toggle Pause | Pause |

### Wayland Support

On Wayland (KDE Plasma, GNOME), shortcuts are registered via `xdg-desktop-portal` GlobalShortcuts interface. You can customize them in your desktop's **System Settings → Shortcuts → LinuxTrack**.

### X11 Support

On X11 sessions, the daemon uses traditional `XGrabKey` for global hotkey capture.

### Starting the Daemon

Enable **"Native Hotkeys"** in the Misc tab of `ltr_gui`, or run manually:

```bash
ltr_hotkeyd --verbose --profile="YourProfileName"
```

---

*For detailed technical implementation notes, see [HANDOFF.md](HANDOFF.md).*
