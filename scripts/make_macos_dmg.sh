#!/usr/bin/env bash
# scripts/make_macos_dmg.sh
set -euo pipefail

APP_NAME="Linuxtrack"
APP_BUNDLE="Linuxtrack.app"
DMG_NAME="Linuxtrack-macOS-Installer.dmg"
VOL_NAME="Linuxtrack Installer"

if [[ $# -ge 1 ]]; then
  STAGING_DIR="$1"
else
  echo "Usage: $0 <staging_dir>"
  exit 1
fi

cd "$STAGING_DIR"

if [[ ! -d "$APP_BUNDLE" ]]; then
  echo "Error: $APP_BUNDLE not found in $(pwd)"
  exit 1
fi

# Clean up any existing DMG
rm -f "$DMG_NAME"

echo "Creating DMG for $APP_NAME..."

# Check if create-dmg is available
if command -v create-dmg &> /dev/null; then
  echo "Using create-dmg to build professional installer..."
  # Note: create-dmg might require a GUI session on some macOS versions,
  # but works fine in most CI environments.
  create-dmg \
    --volname "$VOL_NAME" \
    --window-pos 200 120 \
    --window-size 600 400 \
    --icon-size 100 \
    --icon "$APP_BUNDLE" 175 190 \
    --hide-extension "$APP_BUNDLE" \
    --app-drop-link 425 190 \
    "$DMG_NAME" \
    "$APP_BUNDLE"
else
  echo "create-dmg not found, falling back to basic hdiutil..."
  # Create a temporary directory for the DMG content to add the Applications link
  TMP_DMG_DIR=$(mktemp -d)
  cp -R "$APP_BUNDLE" "$TMP_DMG_DIR/"
  ln -s /Applications "$TMP_DMG_DIR/Applications"
  
  hdiutil create -volname "$VOL_NAME" -srcfolder "$TMP_DMG_DIR" -ov -format UDZO "$DMG_NAME"
  rm -rf "$TMP_DMG_DIR"
fi

echo "DMG created: $(pwd)/$DMG_NAME"
