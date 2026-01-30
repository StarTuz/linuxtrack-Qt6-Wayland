#!/bin/bash
# Local smoke test for Linuxtrack AppImage across multiple distributions
# Usage: ./smoke_test.sh <AppImagePath>

APPIMAGE_PATH="$1"

if [ -z "$APPIMAGE_PATH" ]; then
    echo "Usage: $0 <AppImagePath>"
    exit 1
fi

if [[ ! "$APPIMAGE_PATH" = /* ]]; then
    APPIMAGE_PATH="$(pwd)/$APPIMAGE_PATH"
fi

if [ ! -f "$APPIMAGE_PATH" ]; then
    echo "ERROR: File not found: $APPIMAGE_PATH"
    exit 1
fi

APPIMAGE_NAME=$(basename "$APPIMAGE_PATH")
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VALIDATE_SCRIPT="$SCRIPT_DIR/validate_appimage.sh"

DISTROS=("archlinux:latest" "ubuntu:22.04" "fedora:latest")

echo "=== Starting Multi-Distro Smoke Test ==="
echo "AppImage: $APPIMAGE_NAME"
echo ""

FAILED_DISTROS=()

for DISTRO in "${DISTROS[@]}"; do
    echo "--- Testing on $DISTRO ---"
    
    # Prepare dependencies for validation script
    DEP_CMD=""
    if [[ "$DISTRO" == archlinux* ]]; then
        DEP_CMD="pacman -Syu --noconfirm fuse2 libxcb xcb-util-cursor file libglvnd"
    elif [[ "$DISTRO" == ubuntu* ]]; then
        DEP_CMD="apt-get update && apt-get install -y libfuse2 libxcb1 libxcb-cursor0 file libgl1"
    elif [[ "$DISTRO" == fedora* ]]; then
        DEP_CMD="dnf install -y fuse fuse-libs libxcb file findutils libglvnd-glx libglvnd-opengl"
    fi

    # Run docker
    if docker run --privileged --rm \
        -v "$APPIMAGE_PATH:/app/$APPIMAGE_NAME" \
        -v "$VALIDATE_SCRIPT:/app/validate_appimage.sh" \
        -w /app \
        "$DISTRO" \
        bash -c "$DEP_CMD && ./validate_appimage.sh $APPIMAGE_NAME"; then
        echo "RESULT: $DISTRO PASSED"
    else
        echo "RESULT: $DISTRO FAILED"
        FAILED_DISTROS+=("$DISTRO")
    fi
    echo ""
done

if [ ${#FAILED_DISTROS[@]} -eq 0 ]; then
    echo "=== ALL DISTROS PASSED ==="
    exit 0
else
    echo "=== SMOKE TEST FAILED for: ${FAILED_DISTROS[*]} ==="
    exit 1
fi
