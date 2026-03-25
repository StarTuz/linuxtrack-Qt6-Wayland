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
- **Seamless X-Plane Camera Toggle**: Switch between cockpit and external camera views without disabling TrackIR. One button press for helicopter inspections!

## 🎮 Verified Games & Apps

- ✅ **DCS World** (Proton, with Controller.exe hotkeys)
- ✅ **Elite Dangerous** (Proton 10/Steam Launcher, symmetric range via UDP)
- ✅ **Train Sim World 6** (Proton, full 6DOF via UDP Bridge)
- ✅ **X-Plane 12** (Native Plugin + seamless camera toggle, run `ltr_gui` for One Euro filter)
- ✅ **X4 Foundations** (Via ltr_udp)

## 🆕 Recent in 1.4.0

- **Wayland Support**: Added explicit bundling of Wayland and XKB libraries to the AppImage, ensuring out-of-the-box compatibility with modern distributions like CachyOS.
- **USB Permissions**: Modernized udev rules for TrackIR devices using seat-aware `uaccess` tagging and implemented an automated rule installer (`scripts/install_rules.sh`).
- **Tracking Responsiveness**: Fixed 'hitching' and 'magnetic centering' by making TrackIR 5 'Precision Mode' configurable and tuning the default filter responsiveness.
- **Adaptive Filtering**: Stabilized the time-delta (dt) calculation to prevent scheduling jitter from causing jumps in the smoothing filters.

## 📍 Future Platform Work

macOS support is not currently maintained. There is historical macOS code in the tree, but bringing it back should be treated as a staged platform bring-up, not a small compatibility fix. The current implementation plan lives in [doc/MACOS_ROADMAP.md](doc/MACOS_ROADMAP.md) and intentionally prioritizes:

- build isolation from Linux
- shared-core regression coverage
- webcam and face tracking before TrackIR hardware
- X-Plane support before any claim of broad macOS parity

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

### IR LEDs Turn Off Immediately

If your TrackIR's IR LEDs flash briefly then turn off, your hardware may need a timing delay:

1. Open `ltr_gui` and go to your TrackIR device settings
2. In the **Troubleshooting** section, find **Video On Delay**
3. Set it to `120000` (120ms) or higher
4. Restart tracking

> [!NOTE]
> This setting adds a delay (in microseconds) after enabling video before turning on the IR LEDs. Some TrackIR hardware revisions require this stabilization period.

### X-Plane Plugin: linuxtrack.so Not Found (AppImage)

If X-Plane fails to load the plugin or reports `linuxtrack.so` is missing when using the AppImage:

1. Open `ltr_gui` (from the AppImage).
2. Go to **Misc.** -> **Install X-Plane plugin...**.
3. When prompted, allow the app to install **stable libraries** to `~/.local`.
4. This copies the necessary `.so` files to a location X-Plane can see after the AppImage is closed.

> [!IMPORTANT]
> The X-Plane installer copies the Linux plugin only. Select your Linux X-Plane installation, not a Windows `xplane12.exe` from a dual-boot or Wine setup.
>
> If the installer says `xlinuxtrack9.so` is missing, that build was packaged without X-Plane plugin support. The plugin is only built when the X-Plane SDK headers are installed during compilation.

### X-Plane Quick Start

1. **Create a new profile** in `ltr_gui` (e.g., "X-plane")
2. **Install the X-Plane plugin** via Misc. → Install X-Plane plugin
3. **Start X-Plane** - the plugin loads automatically
4. If TrackIR device is off, just hit **Start** in `ltr_gui`'s Device Setup — tracking will work
5. **Enable One Euro Filter** in Tracking Setup and **Save** your profile
6. **External camera views** now work seamlessly — switch to outside view without pausing tracking!

> [!NOTE]
> The seamless camera toggle works with **vanilla X-Plane cameras only**. Third-party camera plugins (A-Better-Camera, X-Camera) may not be compatible as they handle `view_type` differently.

> [!TIP]
> **Auto-start tracking:** Run `ltr_gui --autostart` (or `-a`) to automatically begin tracking when the GUI launches. Add this to your X-Plane startup script:
>
> ```bash
> ltr_gui --autostart &
> xplane12
> ```

### Jittery/Noisy Tracking (One Euro Filter)

If your tracking feels jittery compared to Windows, you can enable the **One Euro Filter** - an open-source, adaptive smoothing algorithm designed specifically for motion tracking.

> [!IMPORTANT]
> **X-Plane users**: You must run `ltr_gui` alongside X-Plane to access the One Euro filter. The native X-Plane plugin alone does not provide access to these settings.

The filter is available in the **Tracking Setup** tab of your profile with two sliders:

| Parameter | Effect | Range | Default |
|-----------|--------|-------|---------|
| **Smoothness** (`min_cutoff`) | Jitter reduction — slide right for smoother | 0.1-5.0 | 1.0 |
| **Responsiveness** (`beta`) | Fast-move responsiveness — slide right for snappier | 0.0-0.1 | 0.007 |

> [!TIP]
> Start with `min_cutoff = 1.0` and `beta = 0.007`. Increase `min_cutoff` if tracking feels laggy. Increase `beta` if fast movements feel sluggish.

## 🔍 Troubleshooting

### TrackIR Firmware Installation

Download the latest TrackIR software from [trackir.com/downloads](https://www.trackir.com/downloads/) (currently v5.53).

In `ltr_gui` → Misc → **Manage Assets (LAL)**:

> [!IMPORTANT]
> Use the **Extract** button, not the Installer. The Installer method may fail on modern systems. Extract works reliably using native `7z`.

1. Download the TrackIR `.exe` installer from the link above
2. In Manage Assets, click **Extract** and select the downloaded `.exe`
3. The firmware files will be extracted to `~/.config/linuxtrack/tir_firmware/`

### MFC42 Missing (Wine/Proton Games)

If a game complains about missing MFC42:

```bash
# Replace <appid> with your game's Steam App ID
protontricks <appid> mfc42
```

This installs the Microsoft Foundation Classes runtime. Note: Tracking usually works fine even without this fix.

### 3D Tracking View Not Working (NVIDIA)

If the 3D tracking view in `ltr_gui` shows a blank/black screen:

> [!NOTE]
> This may be an NVIDIA driver issue (reported on RTX 4080 Super). The 3D view uses OpenGL ES 3.0 shaders.

**Workarounds to try:**

1. Ensure you have the latest NVIDIA drivers installed
2. Try running with `__GLX_VENDOR_LIBRARY_NAME=nvidia ltr_gui`
3. Check if other OpenGL apps work (e.g., `glxgears`)
4. Tracking still works — the 3D view is purely for visualization

### Webcam Probe Utility

If webcam negotiation is unclear, build and run the shipped probe utility instead of guessing from the GUI alone:

```bash
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/opt/linuxtrack
cmake --build build --target ltr_webcam_probe -j"$(nproc)"
./build/src/ltr_webcam_probe --list
./build/src/ltr_webcam_probe --device "Your Webcam Name" --format MJPG --size 800x600 --fps 1/120 --frames 3
```

The probe prints the requested mode plus the active mode and driver-reported FPS that Linuxtrack actually negotiated. Use it when a camera appears black, falls back to another format, or runs below the requested frame rate.

## 🛠️ Installation

### Supported Install Modes

Linuxtrack supports three practical install paths:

1. **AppImage** for most end users.
2. **Managed source install** via `scripts/setup_linuxtrack.sh`.
3. **Manual CMake install** for developers packaging or debugging locally.

### 1. AppImage (Recommended)

AppImage is the easiest path for most users and keeps Linuxtrack self-contained.

- Download the release AppImage.
- Run it directly: `chmod +x Linuxtrack-*.AppImage && ./Linuxtrack-*.AppImage`
- If you want desktop integration or X-Plane stable libraries, launch `ltr_gui` from the AppImage and use the built-in prompts.

**Generic AppImage:** CI builds are available, but Qt6 ABI differences still matter across distros.
**Arch Linux AppImage:** The Arch-built AppImage is currently the best-tested option for rolling-release systems.

### 2. Managed Source Install

Use the helper script if you want a source install without manually typing every build/install step.

```bash
git clone git@github.com:StarTuz/linuxtrack-Qt6-Wayland.git
cd linuxtrack
scripts/setup_linuxtrack.sh --mode full --prefix /opt/linuxtrack
```

Useful modes:

- `--mode health` checks installs, config prefix, and binary linkage.
- `--mode install` builds if needed and installs to `--prefix`.
- `--mode upgrade` installs to an existing detected prefix.
- `--mode integrate` installs udev rules and desktop entry only.
- `--mode full` runs install plus integration.

### 3. Manual CMake Install

Use this if you are developing, packaging, or debugging the build itself.

**Prerequisites:** Qt6, OpenGL/OpenGLES drivers, libusb-1.0, libmxml, liblo, OpenCV, and Wine build tools if you want Wine bridge support.

```bash
git clone git@github.com:StarTuz/linuxtrack-Qt6-Wayland.git
cd linuxtrack
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/opt/linuxtrack
cmake --build build -j"$(nproc)"
sudo cmake --install build
```

### Uninstall

For source installs, use the shipped uninstaller:

```bash
scripts/uninstall_linuxtrack.sh --prefix /opt/linuxtrack
```

If the install was performed through `setup_linuxtrack.sh`, the uninstaller will use the saved install manifest when available instead of guessing which files to remove.

### Configure Wine/Proton Support

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
