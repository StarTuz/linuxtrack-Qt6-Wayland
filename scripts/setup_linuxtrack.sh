#!/bin/bash
# Linuxtrack Setup & Environment Health Script
# Simplified management for source installs and environment verification.

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Linuxtrack Environment Setup & Health Check${NC}"
echo "============================================"

# Argument parsing
FULL_MODE=false
WITH_MICKEY=false
while [[ $# -gt 0 ]]; do
    case $1 in
        --full)
            FULL_MODE=true
            shift
            ;;
        --mickey)
            WITH_MICKEY=true
            shift
            ;;
        --help|-h)
            echo "Usage: $0 [--full] [--mickey]"
            echo "  --full: Non-interactive build and installation to /opt/linuxtrack"
            echo "  --mickey: Install Mickey (mouse emulation) rules (used with --full)"
            exit 0
            ;;
        *)
            shift
            ;;
    esac
done

if [ "$FULL_MODE" = true ]; then
    echo -e "${YELLOW}Running in FULL mode: Automated Build & Install${NC}"
fi

# OS Detection
IS_ARCH=false
if [ -f /etc/arch-release ]; then
    IS_ARCH=true
    echo "Distro: Arch/EndeavourOS detected."
fi

# Dependency check
check_dependencies() {
    local DEPS=("cmake" "qt6-base" "libusb" "mxml")
    local MISSING_DEPS=()
    for dep in "${DEPS[@]}"; do
        if ! pacman -Qs "$dep" > /dev/null; then
            MISSING_DEPS+=("$dep")
        fi
    done

    if [ ${#MISSING_DEPS[@]} -gt 0 ]; then
        echo -e "${YELLOW}⚠️  Missing build dependencies: ${MISSING_DEPS[*]}${NC}"
        if [ "$FULL_MODE" = true ]; then
            echo "Installing missing dependencies..."
            sudo pacman -S --noconfirm "${MISSING_DEPS[@]}"
        else
            read -p "Do you want to install them now? [y/N] " -n 1 -r
            echo
            if [[ $REPLY =~ ^[Yy]$ ]]; then
                sudo pacman -S "${MISSING_DEPS[@]}"
            fi
        fi
    fi
}

if [ "$IS_ARCH" = true ]; then
    check_dependencies
fi

# 1. Path Discovery
echo -e "\n🔍 Scanning for Linuxtrack installations..."
SEARCH_PATHS=("/opt/linuxtrack" "/usr/local" "/usr" "$HOME/.local")
FOUND_INSTALLS=()

add_unique_install() {
    local new_path="$1"
    for path in "${FOUND_INSTALLS[@]}"; do
        if [ "$path" == "$new_path" ]; then
            return 0
        fi
    done
    FOUND_INSTALLS+=("$new_path")
}

for path in "${SEARCH_PATHS[@]}"; do
    if [ -f "$path/bin/ltr_gui" ]; then
        add_unique_install "$path"
        echo -e "  [+] Found at: $path"
    fi
done

if [ ${#FOUND_INSTALLS[@]} -eq 0 ]; then
    echo -e "  ${YELLOW}No installations found in standard paths.${NC}"
else
    echo -e "  Total found: ${#FOUND_INSTALLS[@]}"
fi

# 2. Config Verification
CONFIG_FILE="$HOME/.config/linuxtrack/linuxtrack1.conf"
if [ -f "$CONFIG_FILE" ]; then
    echo -e "\n📂 Configuration Check:"
    # Use grep -a to handle potential encoding issues if any exist
    CURRENT_PREFIX=$(grep -i "Prefix =" "$CONFIG_FILE" | cut -d'"' -f2)
    echo -e "  Active Prefix in config: $CURRENT_PREFIX"
    
    # Check if this prefix corresponds to a real location
    if [ ! -d "$CURRENT_PREFIX" ]; then
        # Check if it was meant to be the bin directory
        BIN_DIR="${CURRENT_PREFIX}"
        if [[ "$BIN_DIR" != */bin ]]; then
            BIN_DIR="${CURRENT_PREFIX}/bin"
        fi
        
        if [ ! -d "$BIN_DIR" ]; then
            echo -e "  ${RED}⚠️  WARNING: Config Prefix directory does not exist!${NC}"
        else
            echo -e "  ✅ Prefix points to valid directory: $BIN_DIR"
        fi
    else
        echo -e "  ✅ Prefix directory exists."
    fi
else
    echo -e "\n  ⚪ No configuration file found at $CONFIG_FILE"
fi

# 3. Conflict Detection
if [ ${#FOUND_INSTALLS[@]} -gt 1 ]; then
    echo -e "\n${YELLOW}⚠️  Multiple installations detected!${NC}"
    echo "  Running versions from different locations can cause path mismatches"
    echo "  in the Wine bridge and library loading."
    echo "  Consider removing one of them to prevent confusion."
fi

# 4. Dependency Check
echo -e "\n📦 Library Health Check:"
if [ ${#FOUND_INSTALLS[@]} -gt 0 ]; then
    ACTIVE_LTR_GUI="${FOUND_INSTALLS[0]}/bin/ltr_gui"
    MISSING_LIBS=$(ldd "$ACTIVE_LTR_GUI" | grep "not found" || true)
    if [ -z "$MISSING_LIBS" ]; then
        echo -e "  ✅ All dependencies for $ACTIVE_LTR_GUI are resolved."
    else
        echo -e "  ${RED}❌ Error: Missing dependencies for ltr_gui:${NC}"
        echo "$MISSING_LIBS"
    fi
else
    echo "  (Skip: No binaries to check)"
fi

# 5. TrackIR Firmware Check
echo -e "\n🔌 TrackIR Firmware Check:"
FIRMWARE_DIR="$HOME/.config/linuxtrack/tir_firmware"
if [ -d "$FIRMWARE_DIR" ] && [ -f "$FIRMWARE_DIR/TIRViews.dll" ]; then
    echo -e "  ✅ TrackIR firmware found."
else
    echo -e "  ⚪ TrackIR firmware not found (normal if not using TrackIR)."
fi

# 6. Prefix Auto-Fix
fix_prefix() {
    if [ ${#FOUND_INSTALLS[@]} -eq 0 ]; then
        echo "No installations found to set as prefix."
        return
    fi
    
    # Use the first one found or let user choose if multiple
    TARGET_INST=""
    if [ ${#FOUND_INSTALLS[@]} -eq 1 ]; then
        TARGET_INST="${FOUND_INSTALLS[0]}"
    else
        echo -e "\nMultiple installs found. Choose one to set as active prefix:"
        for i in "${!FOUND_INSTALLS[@]}"; do
            echo "  $((i+1))) ${FOUND_INSTALLS[$i]}"
        done
        read -p "Selection [1-${#FOUND_INSTALLS[@]}]: " SEL
        TARGET_INST="${FOUND_INSTALLS[$((SEL-1))]}"
    fi
    
    TARGET_PREFIX="$TARGET_INST/bin"
    echo "Updating config prefix to: $TARGET_PREFIX"
    
    if [ -f "$CONFIG_FILE" ]; then
        # Backup
        cp "$CONFIG_FILE" "${CONFIG_FILE}.bak"
        # Update using sed - handles various spacings
        sed -i "s|^[Pp]refix.*=.*|Prefix = \"$TARGET_PREFIX\"|" "$CONFIG_FILE"
        echo -e "  ✅ Config updated. Backup created at ${CONFIG_FILE}.bak"
    else
        echo -e "  ❌ Config file not found, cannot update."
    fi
}

if [ -f "$CONFIG_FILE" ] && [ ${#FOUND_INSTALLS[@]} -gt 0 ]; then
    # Check if current prefix matches any found installs
    MATCH_FOUND=false
    for inst in "${FOUND_INSTALLS[@]}"; do
        if [[ "$CURRENT_PREFIX" == "$inst"* ]]; then
            MATCH_FOUND=true
            break
        fi
    done
    
    if [ "$MATCH_FOUND" = false ]; then
        echo -e "\n${YELLOW}⚠️  Config mismatch detected!${NC}"
        echo "  The active prefix ($CURRENT_PREFIX) doesn't match any found installations."
        read -p "Do you want to fix the config prefix? [y/N] " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            fix_prefix
        fi
    fi
fi

# 7. Build & Installation from current source
perform_build() {
    echo -e "\n🛠️  Starting build process..."
    if ! command -v cmake &> /dev/null; then
        echo -e "${RED}❌ Error: 'cmake' not found. Please install CMake first.${NC}"
        return 1
    fi
    
    # Determine project root (one level up from scripts/)
    local SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    local PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
    
    # Check if we should use build or build_test
    local BUILD_TARGET="$PROJECT_ROOT/build"
    mkdir -p "$BUILD_TARGET"
    
    echo "Project root: $PROJECT_ROOT"
    echo "Build directory: $BUILD_TARGET"
    echo "Configuring with CMake..."
    if ! cmake -S "$PROJECT_ROOT" -B "$BUILD_TARGET" -DCMAKE_INSTALL_PREFIX=/opt/linuxtrack; then
        echo -e "${RED}❌ CMake configuration failed.${NC}"
        return 1
    fi
    
    echo "Compiling..."
    if ! cmake --build "$BUILD_TARGET" -j$(nproc); then
        echo -e "${RED}❌ Compilation failed.${NC}"
        return 1
    fi
    
    echo -e "${GREEN}✅ Build completed successfully.${NC}"
    BUILD_DIR="$BUILD_TARGET"
    return 0
}

perform_install() {
    local target="$1"
    echo -e "\n🚀 Preparing to install/upgrade to: $target"
    
    # Check if we have a build directory
    BUILD_DIR=""
    if [ -d "build" ] && [ -f "build/src/qt_gui/ltr_gui" ]; then
        BUILD_DIR="build"
    elif [ -d "build_test" ] && [ -f "build_test/src/qt_gui/ltr_gui" ]; then
        BUILD_DIR="build_test"
    fi
    
    if [ -z "$BUILD_DIR" ]; then
        if [ "$FULL_MODE" = true ]; then
             if ! perform_build; then return 1; fi
        else
            echo -e "${YELLOW}No compiled binaries found.${NC}"
            read -p "Do you want to BUILD Linuxtrack from source now? [y/N] " -n 1 -r
            echo
            if [[ $REPLY =~ ^[Yy]$ ]]; then
                if ! perform_build; then return 1; fi
            else
                return 1
            fi
        fi
    fi
    
    echo -e "Using build from: $BUILD_DIR"
    
    # Check permissions
    INSTALL_CMD="cmake --install $BUILD_DIR --prefix $target"
    if [ ! -w "$target" ] && [ ! -w "$(dirname "$target")" ]; then
        echo "Requires sudo for installation to $target"
        INSTALL_CMD="sudo $INSTALL_CMD"
    fi
    
    echo "Running: $INSTALL_CMD"
    if $INSTALL_CMD; then
        echo -e "${GREEN}✅ Installation/Upgrade successful!${NC}"
        # Update prefix automatically after install
        add_unique_install "$target"
        fix_prefix
    else
        echo -e "${RED}❌ Installation failed.${NC}"
    fi
}

if [ "$FULL_MODE" = true ]; then
    perform_install "/opt/linuxtrack"
    setup_integration
    echo -e "\n${GREEN}🚀 Full installation complete!${NC}"
    exit 0
fi

echo -e "\n🚀 Build & Upgrade Options:"
if [ ${#FOUND_INSTALLS[@]} -gt 0 ]; then
    echo "  An existing installation was found."
    read -p "Do you want to UPGRADE an existing installation? [y/N] " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        # Use same selection logic as fix_prefix
        TARGET_UPGRADE=""
        if [ ${#FOUND_INSTALLS[@]} -eq 1 ]; then
            TARGET_UPGRADE="${FOUND_INSTALLS[0]}"
        else
            echo "Choose installation to upgrade:"
            for i in "${!FOUND_INSTALLS[@]}"; do echo "  $((i+1))) ${FOUND_INSTALLS[$i]}"; done
            read -p "Selection: " SEL
            TARGET_UPGRADE="${FOUND_INSTALLS[$((SEL-1))]}"
        fi
        perform_install "$TARGET_UPGRADE"
    fi
else
    read -p "No installations found. Do you want to INSTALL Linuxtrack from current source? [y/N] " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        read -p "Enter installation path (default: /opt/linuxtrack): " NEW_PATH
        NEW_PATH=${NEW_PATH:-/opt/linuxtrack}
        perform_install "$NEW_PATH"
    fi
fi

# 8. System Integration (Interactive)
setup_integration() {
    echo -e "\n🛠️  System Integration Setup"
    
    # udev rules
    UDEV_SRC="share/linuxtrack/99-TIR.rules"
    if [ ! -f "$UDEV_SRC" ]; then UDEV_SRC="src/99-TIR.rules"; fi 
    
    if [ -f "$UDEV_SRC" ]; then
        echo -e "  (requires sudo) Installing TrackIR udev rules..."
        sudo cp "$UDEV_SRC" /etc/udev/rules.d/
        echo -e "  ✅ TrackIR rules installed."
        
        # 99-Mickey.rules (Optional)
        MICKEY_UDEV="${UDEV_SRC/TIR/Mickey}"
        if [ -f "$MICKEY_UDEV" ]; then
            echo -e "\n  ℹ️  Mickey Rules Explanation:"
            echo "     Mickey is the component that allows Linuxtrack to emulate a mouse or joystick."
            echo "     This is useful for games that don't natively support head tracking."
            echo "     The Mickey rules allow this emulation to work without root privileges."
            
            INSTALL_MICKEY=false
            if [ "$FULL_MODE" = true ]; then
                if [ "$WITH_MICKEY" = true ]; then
                    INSTALL_MICKEY=true
                else
                    echo "     (Skip: Mickey rules not requested in --full mode)"
                fi
            else
                read -p "     Do you want to install Mickey (mouse emulation) rules? [y/N] " -n 1 -r
                echo
                if [[ $REPLY =~ ^[Yy]$ ]]; then
                    INSTALL_MICKEY=true
                fi
            fi

            if [ "$INSTALL_MICKEY" = true ]; then
                echo "     Installing Mickey udev rules..."
                sudo cp "$MICKEY_UDEV" /etc/udev/rules.d/
                echo -e "     ✅ Mickey rules installed."
            fi
        fi
        
        sudo udevadm control --reload-rules
        sudo udevadm trigger
        echo -e "  ✅ udev rules reloaded."
    else
        echo -e "  ${YELLOW}⚠️  Udev rules source not found (99-TIR.rules).${NC}"
    fi
    
    # User groups
    if [ "$IS_ARCH" = true ]; then
        TARGET_GROUP="uucp"
    else
        TARGET_GROUP="plugdev"
    fi
    
    if groups "$USER" | grep -q "\b$TARGET_GROUP\b"; then
        echo -e "  ✅ User $USER is already in $TARGET_GROUP."
    else
        echo -e "  (requires sudo) Adding $USER to $TARGET_GROUP group..."
        sudo usermod -aG "$TARGET_GROUP" "$USER"
        echo -e "  ✅ User added. ${YELLOW}You may need to log out and back in for this to take effect.${NC}"
    fi

    # Desktop files
    DESKTOP_DIR="$HOME/.local/share/applications"
    mkdir -p "$DESKTOP_DIR"
    DP_SRC="src/linuxtrack.desktop"
    if [ -f "$DP_SRC" ]; then
        echo -e "  Installing desktop entry to $DESKTOP_DIR"
        if [ ${#FOUND_INSTALLS[@]} -gt 0 ]; then
            EXE_PATH="${FOUND_INSTALLS[0]}/bin/ltr_gui"
            sed "s|^Exec=.*|Exec=$EXE_PATH|" "$DP_SRC" > "$DESKTOP_DIR/linuxtrack.desktop"
        else
            cp "$DP_SRC" "$DESKTOP_DIR/"
        fi
        echo -e "  ✅ Desktop entry installed."
    fi
}

echo
read -p "Do you want to run system integration (udev, groups, desktop files)? [y/N] " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    setup_integration
fi

echo -e "\n🏁 Health Check complete."
