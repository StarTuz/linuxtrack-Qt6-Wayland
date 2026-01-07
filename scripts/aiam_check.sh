#!/bin/bash
# AIAM Absolute Verification Script for Linuxtrack
# This script provides agents with verified state proof before Tier 2/3 actions.

CONFIG_FILE="$HOME/.config/linuxtrack/linuxtrack1.conf"
FIRMWARE_DIR="$HOME/.config/linuxtrack/tir_firmware"
XPLANE_PLUGIN_DIR="$HOME/.x-plane/Resources/plugins/xlinuxtrack9"

echo "🔍 [AIAM] Initiating Linuxtrack Verification..."
echo ""

# 1. Check Config Existence
echo "📁 Configuration Check:"
if [ -f "$CONFIG_FILE" ]; then
    echo "  ✅ Config file exists: $CONFIG_FILE"
    # Extract current profile if possible
    CURRENT_PROFILE=$(grep -oP '^\[Profile:([^\]]+)' "$CONFIG_FILE" | head -1 | sed 's/\[Profile://')
    if [ ! -z "$CURRENT_PROFILE" ]; then
        echo "  📌 First profile found: $CURRENT_PROFILE"
    fi
else
    echo "  ❌ CONFIG ERROR: $CONFIG_FILE not found"
    echo "     (This is normal if linuxtrack hasn't been run yet)"
fi
echo ""

# 2. Verify Process State
echo "📡 Process State:"
LTR_SERVER_PID=$(pgrep -f "ltr_server1")
LTR_GUI_PID=$(pgrep -f "ltr_gui")
LTR_HOTKEYD_PID=$(pgrep -f "ltr_hotkeyd")
LTR_UDP_PID=$(pgrep -f "ltr_udp")

if [ ! -z "$LTR_SERVER_PID" ]; then echo "  ✅ ltr_server1 is RUNNING (PID: $LTR_SERVER_PID)"; else echo "  ⚪ ltr_server1 is NOT running"; fi
if [ ! -z "$LTR_GUI_PID" ]; then echo "  ✅ ltr_gui is RUNNING (PID: $LTR_GUI_PID)"; else echo "  ⚪ ltr_gui is NOT running"; fi
if [ ! -z "$LTR_HOTKEYD_PID" ]; then echo "  ✅ ltr_hotkeyd is RUNNING (PID: $LTR_HOTKEYD_PID)"; else echo "  ⚪ ltr_hotkeyd is NOT running"; fi
if [ ! -z "$LTR_UDP_PID" ]; then echo "  ✅ ltr_udp is RUNNING (PID: $LTR_UDP_PID)"; else echo "  ⚪ ltr_udp is NOT running"; fi
echo ""

# 3. TrackIR Firmware Status
echo "🔌 TrackIR Firmware:"
if [ -d "$FIRMWARE_DIR" ]; then
    if [ -f "$FIRMWARE_DIR/TIRViews.dll" ]; then
        echo "  ✅ TIRViews.dll present"
    else
        echo "  ⚠️  TIRViews.dll NOT found (required for TrackIR devices)"
    fi
    if [ -f "$FIRMWARE_DIR/gamedata.txt" ]; then
        echo "  ✅ gamedata.txt present"
    else
        echo "  ⚠️  gamedata.txt NOT found"
    fi
else
    echo "  ⚪ Firmware directory not found: $FIRMWARE_DIR"
    echo "     (Normal if using webcam/facetracker instead of TrackIR)"
fi
echo ""

# 4. X-Plane Plugin Installation
echo "✈️  X-Plane Plugin:"
if [ -d "$XPLANE_PLUGIN_DIR" ]; then
    if [ -f "$XPLANE_PLUGIN_DIR/lin_x64/xlinuxtrack9.xpl" ]; then
        echo "  ✅ 64-bit plugin installed"
    else
        echo "  ⚠️  64-bit plugin NOT found"
    fi
else
    # Try alternative paths
    ALT_XPLANE=$(find "$HOME" -maxdepth 3 -type d -name "X-Plane*" 2>/dev/null | head -1)
    if [ ! -z "$ALT_XPLANE" ] && [ -d "$ALT_XPLANE/Resources/plugins/xlinuxtrack9" ]; then
        echo "  ✅ Plugin found at: $ALT_XPLANE/Resources/plugins/xlinuxtrack9"
    else
        echo "  ⚪ X-Plane plugin directory not found (normal if X-Plane not installed)"
    fi
fi
echo ""

# 5. AppImage Mount Check (No-Touch Zone verification)
echo "🛡️  No-Touch Zone Scan:"
APPIMAGE_MOUNT=$(mount | grep -i "linuxtrack.*appimage" | head -1)
if [ ! -z "$APPIMAGE_MOUNT" ]; then
    echo "  ⚠️  WARNING: AppImage is currently mounted"
    echo "     $APPIMAGE_MOUNT"
    echo "     Avoid modifying mounted AppImage contents!"
else
    echo "  ✅ No active AppImage mounts detected"
fi

# Check for legacy binaries
if [ -f "/opt/linuxtrack/bin/ltr_gui" ]; then
    echo "  ℹ️  System installation found at /opt/linuxtrack/"
fi
if [ -f "$HOME/.local/bin/ltr_gui" ]; then
    echo "  ℹ️  User installation found at ~/.local/bin/"
fi
echo ""

echo "🏁 [AIAM] Linuxtrack Verification Complete."
