#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 /path/to/Linuxtrack.app-or-install-prefix" >&2
  exit 2
fi

input_path="$1"
if [[ ! -e "$input_path" ]]; then
  echo "Path does not exist: $input_path" >&2
  exit 1
fi

if [[ "$(basename "$input_path")" == "Linuxtrack.app" ]]; then
  bundle_path="$input_path"
else
  bundle_path="${input_path%/}/Linuxtrack.app"
fi

if [[ ! -d "$bundle_path" ]]; then
  echo "Linuxtrack.app not found under: $input_path" >&2
  exit 1
fi

required_files=(
  "Contents/Info.plist"
  "Contents/MacOS/Linuxtrack"
  "Contents/Resources/linuxtrack.icns"
  "Contents/Resources/linuxtrack/linuxtrack1.conf"
  "Contents/Resources/linuxtrack/sources_mac.txt"
  "Contents/Resources/linuxtrack/help/ltr_gui/help.qhc"
  "Contents/Resources/linuxtrack/help/ltr_gui/help.qch"
  "Contents/Resources/linuxtrack/sphere.obj"
  "Contents/Resources/linuxtrack/spec.txt"
)

missing=0
for rel in "${required_files[@]}"; do
  abs="${bundle_path}/${rel}"
  if [[ ! -e "$abs" ]]; then
    echo "Missing: $abs" >&2
    missing=1
  fi
done

plist_path="${bundle_path}/Contents/Info.plist"
if [[ -f "$plist_path" ]]; then
  if ! grep -q "NSCameraUsageDescription" "$plist_path"; then
    echo "Missing NSCameraUsageDescription in Info.plist" >&2
    missing=1
  fi
  if ! grep -q "org.linuxtrack.ltr_gui.experimental" "$plist_path"; then
    echo "Missing experimental bundle identifier in Info.plist" >&2
    missing=1
  fi
fi

if [[ $missing -ne 0 ]]; then
  exit 1
fi

echo "Bundle layout OK: $bundle_path"
