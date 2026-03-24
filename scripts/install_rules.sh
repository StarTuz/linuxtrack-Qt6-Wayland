#!/bin/bash
set -e

# Script to install Linuxtrack udev rules for TrackIR/SmartNav devices.
# This ensures that the current user has permission to access the hardware.

RULE_FILE="src/99-TIR.rules"
DEST_PATH="/etc/udev/rules.d/99-TIR.rules"

# Ensure we are in the project root
if [ ! -f "$RULE_FILE" ]; then
    echo "Error: Please run this script from the project root directory."
    exit 1
fi

if [ "$EUID" -ne 0 ]; then
    echo "This script requires root privileges to copy files to /etc/udev/rules.d/"
    echo "Please run with sudo: sudo $0"
    exit 1
fi

echo "--> Installing Linuxtrack udev rules to $DEST_PATH..."
cp "$RULE_FILE" "$DEST_PATH"
chmod 644 "$DEST_PATH"

echo "--> Reloading udev rules..."
udevadm control --reload-rules
udevadm trigger

echo "--> Success! Please unplug and replug your TrackIR/SmartNav device."
echo "    If you still encounter permission issues, check if your user is in the 'users' group."
