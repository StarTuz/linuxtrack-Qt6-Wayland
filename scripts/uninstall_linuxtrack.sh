#!/bin/bash
# Linuxtrack Uninstallation Script
# Cleans up all Linuxtrack components from the system.

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${RED}Linuxtrack Uninstaller${NC}"
echo "======================"

# 1. Path Discovery (Reuse discovery logic to find what to delete)
SEARCH_PATHS=("/opt/linuxtrack" "/usr/local" "/usr" "$HOME/.local")
FOUND_INSTALLS=()

for path in "${SEARCH_PATHS[@]}"; do
    if [ -f "$path/bin/ltr_gui" ]; then
        FOUND_INSTALLS+=("$path")
    fi
done

if [ ${#FOUND_INSTALLS[@]} -eq 0 ]; then
    echo "No standard installations found. Proceeding with system-wide cleanup."
else
    echo "The following installations were found:"
    for i in "${!FOUND_INSTALLS[@]}"; do
        echo "  $((i+1))) ${FOUND_INSTALLS[$i]}"
    done
fi

remove_installation() {
    local target="$1"
    echo -e "\n🗑️  Removing installation at: $target"
    
    # We need to be careful with /usr/local and /usr
    if [ "$target" == "/usr" ] || [ "$target" == "/usr/local" ]; then
        echo -e "${YELLOW}⚠️  Warning: Target is a core system path ($target).${NC}"
        echo "Only specific Linuxtrack files will be removed."
        
        SUDO=""
        if [ ! -w "$target/bin" ]; then SUDO="sudo"; fi
        
        $SUDO rm -f "$target/bin/ltr_"*
        $SUDO rm -f "$target/bin/mickey"
        $SUDO rm -rf "$target/lib/linuxtrack"
        $SUDO rm -rf "$target/share/linuxtrack"
    else
        SUDO=""
        if [ ! -w "$target" ]; then SUDO="sudo"; fi
        $SUDO rm -rf "$target"
    fi
    echo -e "  ✅ Installation removed."
}

# Ask to remove found installs
if [ ${#FOUND_INSTALLS[@]} -gt 0 ]; then
    read -p "Do you want to remove the detected installations? [y/N] " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        for inst in "${FOUND_INSTALLS[@]}"; do
            remove_installation "$inst"
        done
    fi
fi

# 2. Udev Rules Cleanup
echo -e "\n🛠️  Cleaning up system integration..."
UDEV_RULES=("/etc/udev/rules.d/99-TIR.rules" "/etc/udev/rules.d/99-Mickey.rules")
for rule in "${UDEV_RULES[@]}"; do
    if [ -f "$rule" ]; then
        echo "Removing $rule..."
        sudo rm "$rule"
    fi
done
sudo udevadm control --reload-rules

# 3. Desktop Entry Cleanup
DESKTOP_FILE="$HOME/.local/share/applications/linuxtrack.desktop"
if [ -f "$DESKTOP_FILE" ]; then
    echo "Removing desktop entry..."
    rm "$DESKTOP_FILE"
fi

# 4. AppImage Stable Library Cleanup
STABLE_LIBS="$HOME/.local/lib/linuxtrack"
if [ -d "$STABLE_LIBS" ]; then
    echo "Removing stable libraries for AppImage..."
    rm -rf "$STABLE_LIBS"
fi

# 5. User Data Cleanup (Optional)
CONFIG_DIR="$HOME/.config/linuxtrack"
if [ -d "$CONFIG_DIR" ]; then
    echo -e "\n${YELLOW}📁 User data found at $CONFIG_DIR (Profiles, settings, firmware).${NC}"
    read -p "Do you want to PERMANENTLY remove your user configuration and profiles? [y/N] " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        rm -rf "$CONFIG_DIR"
        echo "  ✅ User data removed."
    else
        echo "  📂 User data preserved."
    fi
fi

echo -e "\n${GREEN}🏁 Linuxtrack has been uninstalled.${NC}"
