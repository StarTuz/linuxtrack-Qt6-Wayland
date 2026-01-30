# Pre-Release Checklist

Use this checklist before tagging a new release.

## Build Verification

- [ ] Clean build succeeds: `rm -rf build && mkdir build && cd build && cmake .. && make -j$(nproc)`
- [ ] No new compiler warnings in changed files
- [ ] Version updated in `CMakeLists.txt`
- [ ] Version updated in `src/ltr_server1.c` (if hardcoded)
- [ ] Release notes added to `HANDOFF.md`

## AppImage Validation

- [ ] `packaging/appimage/make_appimage.sh` completes without errors
- [ ] Run validation: `./scripts/validate_appimage.sh Linuxtrack*.AppImage`
- [ ] AppImage launches on dev machine
- [ ] AppImage launches in clean Docker container (see below)

## Quick Docker Tests

```bash
# Test on clean Arch
docker run --rm -v $(pwd):/app:ro archlinux:latest bash -c "
  pacman -Syu --noconfirm fuse2 libxcb file findutils
  cd /app && ./scripts/validate_appimage.sh Linuxtrack*.AppImage
"

# Test on clean Ubuntu 22.04
docker run --rm -v $(pwd):/app:ro ubuntu:22.04 bash -c "
  apt-get update && apt-get install -y libfuse2 libxcb1 file
  cd /app && ./scripts/validate_appimage.sh Linuxtrack*.AppImage
"
```

## Hardware Testing (if driver changes)

- [ ] TrackIR device detected: `lsusb | grep 131d`
- [ ] Permissions work without sudo (udev rules applied)
- [ ] Tracking data visible in ltr_gui
- [ ] Data flows to test application (X-Plane, wine game, etc.)

## Cross-Distro (Major Releases)

- [ ] Tested on Ubuntu/Debian-based
- [ ] Tested on Arch/EndeavourOS/Manjaro
- [ ] Tested on Fedora/RHEL-based (if possible)

## Final Steps

- [ ] Commit all changes
- [ ] Tag release: `git tag -a vX.Y.Z -m "Release vX.Y.Z"`
- [ ] Push: `git push origin main --tags`
- [ ] Upload AppImage to GitHub release
- [ ] Verify CI passes on the new tag
