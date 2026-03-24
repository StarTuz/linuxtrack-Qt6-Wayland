#!/bin/bash
# Linuxtrack Udev Rules Installer

RULES_FILE="src/99-TIR.rules"
DEST_DIR="/etc/udev/rules.d"

if [ ! -f "$RULES_FILE" ]; then
    echo "Error: $RULES_FILE not found. Please run this script from the project root."
    exit 1
fi

echo "Copying $RULES_FILE to $DEST_DIR..."
sudo cp "$RULES_FILE" "$DEST_DIR/"

echo "Reloading udev rules..."
sudo udevadm control --reload-rules
sudo udevadm trigger

echo "Done! If your TrackIR device is plugged in, please replug it or run 'ltr_gui' to test."
