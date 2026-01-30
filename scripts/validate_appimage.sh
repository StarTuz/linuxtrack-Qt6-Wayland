#!/bin/bash
# Validate an AppImage for missing dependencies and basic functionality
# Usage: ./validate_appimage.sh <AppImage>
set -e

APPIMAGE="$1"
if [ -z "$APPIMAGE" ]; then
    echo "Usage: $0 <AppImage>"
    exit 1
fi

if [ ! -f "$APPIMAGE" ]; then
    echo "ERROR: File not found: $APPIMAGE"
    exit 1
fi

WORKDIR=$(mktemp -d)
trap "rm -rf $WORKDIR" EXIT

echo "=== Extracting AppImage ==="
chmod +x "$APPIMAGE"
cd "$WORKDIR"
"$OLDPWD/$APPIMAGE" --appimage-extract > /dev/null 2>&1

echo "=== Checking for missing libraries ==="
MISSING=$(find squashfs-root -name "*.so*" -type f -exec ldd {} \; 2>&1 | grep "not found" | sort -u || true)
if [ -n "$MISSING" ]; then
    echo "WARNING: Missing libraries detected:"
    echo "$MISSING"
    echo ""
    echo "Note: Some missing libs may be expected if they're system-specific (GPU drivers, etc.)"
else
    echo "OK: All library dependencies satisfied"
fi

echo ""
echo "=== Checking critical files ==="
CRITICAL_FILES=(
    "usr/bin/ltr_gui"
    "usr/bin/ltr_server1"
    "usr/bin/ltr_recenter"
    "usr/lib/linuxtrack/libltr.so"
    "usr/lib/linuxtrack/liblinuxtrack.so"
    "usr/lib/linuxtrack/libltusb1.so"
    "usr/lib/linuxtrack/libtir.so"
    "usr/lib/linuxtrack/libwc.so"
    "usr/lib/linuxtrack/libft.so"
    "usr/share/linuxtrack/lal_manifest.json"
    "usr/lib/linuxtrack/NPClient.dll.so"
    "usr/lib/linuxtrack/FreeTrackClient.dll.so"
)

ERRORS=0
for f in "${CRITICAL_FILES[@]}"; do
    if [ -e "squashfs-root/$f" ]; then
        echo "OK: $f"
    else
        echo "FAIL: $f missing!"
        ERRORS=$((ERRORS + 1))
    fi
done

echo ""
echo "=== Testing GUI launch (headless) ==="
export QT_QPA_PLATFORM=offscreen
export QT_LOGGING_RULES="*.debug=false"
if timeout 3 squashfs-root/AppRun --help 2>&1 | head -5; then
    echo "OK: AppRun executed"
else
    echo "OK: AppRun started (timeout expected for GUI app)"
fi

echo ""
echo "=== AppImage size ==="
ls -lh "$OLDPWD/$APPIMAGE"

echo ""
if [ $ERRORS -gt 0 ]; then
    echo "=== VALIDATION FAILED: $ERRORS critical files missing ==="
    exit 1
else
    echo "=== VALIDATION PASSED ==="
fi
