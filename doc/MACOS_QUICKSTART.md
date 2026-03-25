# Linuxtrack macOS QuickStart (Experimental)

This guide helps macOS users install and run Linuxtrack. As these are un-signed experimental builds, you will need to perform a few extra steps to bypass macOS security restrictions.

## 1. Installation

1. Download the `Linuxtrack-macOS-Installer.dmg` from the latest release.
2. Double-click the `.dmg` file to mount it.
3. Drag the **Linuxtrack** icon into the **Applications** folder shortcut.
4. Eject the DMG.

## 2. Bypassing Gatekeeper & Quarantine

Because the app is not signed by an Apple Developer certificate, macOS will block it by default.

### Method A: Right-Click Open (Easiest)

1. Right-click (or Control-click) `Linuxtrack.app` in Finder.
2. Select **Open**.
3. A dialog will appear saying "macOS cannot verify the developer...". Click **Open** again.
4. You only need to do this once.

### Method B: Terminal (If Method A fails)

If you see a "damaged" error (common for downloaded zips), run this in Terminal:

```bash
xattr -dr com.apple.quarantine /Applications/Linuxtrack.app
```

## 3. Granting Permissions

Linuxtrack requires specific system permissions to function:

### Camera Access (for Webcams)

- When you first start tracking with a webcam, macOS should prompt you to allow Camera access.
- If it doesn't, go to **System Settings** -> **Privacy & Security** -> **Camera** and ensure Linuxtrack is enabled.

### Input Monitoring (for Hotkeys)

- If you want to use global hotkeys (Recenter/Pause) while in other apps, you must grant **Input Monitoring** permission.
- Go to **System Settings** -> **Privacy & Security** -> **Input Monitoring**.
- Click the **+** button and add `Linuxtrack.app`.

## 4. Hardware Support (TrackIR)

Linuxtrack uses `libusb` to communicate with TrackIR hardware.

- In current experimental builds, you may need to have [Homebrew](https://brew.sh/) installed to provide core libraries:

```bash
brew install libusb libmxml
```

- If the app fails to find your device, ensure no other head-tracking software is running.

---

**Note:** This is an experimental build. Please report issues on GitHub!
