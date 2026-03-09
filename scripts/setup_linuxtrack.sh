#!/bin/bash
# Linuxtrack install, upgrade, integration, and health-check helper.

set -euo pipefail

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
DEFAULT_BUILD_DIR="$PROJECT_ROOT/build"

MODE="health"
PREFIX="/opt/linuxtrack"
BUILD_DIR="$DEFAULT_BUILD_DIR"
ASSUME_YES=false
SKIP_DEPS=false
WITH_MICKEY=false

DISTRO_ID="unknown"
PKG_MANAGER=""

FOUND_INSTALLS=()
ACTIVE_PREFIX=""

usage() {
    cat <<EOF
Usage: $(basename "$0") [options]

Modes:
  --mode health       Check install/config state only (default)
  --mode build        Configure and build only
  --mode install      Build if needed, install to --prefix
  --mode upgrade      Install to an existing detected prefix or --prefix
  --mode integrate    Install udev rules and desktop entry only
  --mode full         Install and then run integration

Options:
  --prefix PATH       Installation prefix (default: /opt/linuxtrack)
  --build-dir PATH    Out-of-tree build directory (default: ./build)
  --yes, -y           Non-interactive mode where possible
  --skip-deps         Do not attempt dependency installation
  --with-mickey       Install Mickey udev rules during integration
  --help, -h          Show this help

Examples:
  scripts/setup_linuxtrack.sh --mode install --prefix /opt/linuxtrack
  scripts/setup_linuxtrack.sh --mode full --prefix /usr/local --yes
  scripts/setup_linuxtrack.sh --mode integrate --with-mickey
EOF
}

log() {
    printf '%b\n' "$1"
}

confirm() {
    local prompt="$1"
    if [ "$ASSUME_YES" = true ]; then
        return 0
    fi
    read -r -p "$prompt [y/N] " reply
    [[ "$reply" =~ ^[Yy]$ ]]
}

detect_distro() {
    if [ -f /etc/os-release ]; then
        # shellcheck disable=SC1091
        . /etc/os-release
        DISTRO_ID="${ID:-unknown}"
    fi

    if command -v pacman >/dev/null 2>&1; then
        PKG_MANAGER="pacman"
    elif command -v apt-get >/dev/null 2>&1; then
        PKG_MANAGER="apt"
    elif command -v dnf >/dev/null 2>&1; then
        PKG_MANAGER="dnf"
    elif command -v zypper >/dev/null 2>&1; then
        PKG_MANAGER="zypper"
    fi
}

dependency_list() {
    case "$PKG_MANAGER" in
        pacman)
            printf '%s\n' cmake qt6-base qt6-tools libusb mxml liblo opencv nlohmann-json wine
            ;;
        apt)
            printf '%s\n' \
                cmake build-essential pkg-config \
                qt6-base-dev qt6-tools-dev qt6-tools-dev-tools qt6-l10n-tools \
                libmxml-dev libusb-1.0-0-dev liblo-dev libopencv-dev \
                zlib1g-dev libv4l-dev libgl1-mesa-dev libglu1-mesa-dev \
                libx11-dev libxkbcommon-dev nlohmann-json3-dev wine
            ;;
        dnf)
            printf '%s\n' \
                cmake gcc gcc-c++ pkgconf-pkg-config \
                qt6-qtbase-devel qt6-qttools-devel \
                mxml-devel libusb1-devel liblo-devel opencv-devel \
                zlib-devel libv4l-devel mesa-libGL-devel mesa-libGLU-devel \
                libX11-devel libxkbcommon-devel nlohmann-json-devel wine
            ;;
        zypper)
            printf '%s\n' \
                cmake gcc gcc-c++ pkgconf-pkg-config \
                libqt6-qtbase-devel libqt6-qttools-devel \
                mxml-devel libusb-1_0-devel liblo-devel opencv-devel \
                zlib-devel libv4l-devel Mesa-libGL-devel glu-devel \
                libX11-devel libxkbcommon-devel nlohmann_json-devel wine
            ;;
    esac
}

install_dependencies() {
    if [ "$SKIP_DEPS" = true ]; then
        return 0
    fi

    detect_distro
    if [ -z "$PKG_MANAGER" ]; then
        log "${YELLOW}⚠️  No supported package manager detected. Install dependencies manually.${NC}"
        return 0
    fi

    mapfile -t deps < <(dependency_list)
    if [ "${#deps[@]}" -eq 0 ]; then
        return 0
    fi

    log "\n${GREEN}📦 Dependency profile:${NC} $PKG_MANAGER ($DISTRO_ID)"
    log "Recommended packages:"
    printf '  %s\n' "${deps[@]}"

    if [ "$ASSUME_YES" != true ] && ! confirm "Install missing packages automatically?"; then
        return 0
    fi

    case "$PKG_MANAGER" in
        pacman)
            sudo pacman -S --needed ${ASSUME_YES:+--noconfirm} "${deps[@]}"
            ;;
        apt)
            sudo apt-get update
            sudo apt-get install -y "${deps[@]}"
            ;;
        dnf)
            sudo dnf install -y "${deps[@]}"
            ;;
        zypper)
            sudo zypper install -y "${deps[@]}"
            ;;
    esac
}

add_unique_install() {
    local path="$1"
    local existing
    for existing in "${FOUND_INSTALLS[@]}"; do
        if [ "$existing" = "$path" ]; then
            return
        fi
    done
    FOUND_INSTALLS+=("$path")
}

scan_installations() {
    FOUND_INSTALLS=()
    local search_paths=("/opt/linuxtrack" "/usr/local" "/usr" "$HOME/.local")
    local path
    for path in "${search_paths[@]}"; do
        if [ -x "$path/bin/ltr_gui" ]; then
            add_unique_install "$path"
        fi
    done
}

show_health() {
    scan_installations

    log "${GREEN}Linuxtrack Health Check${NC}"
    log "======================="

    if [ "${#FOUND_INSTALLS[@]}" -eq 0 ]; then
        log "${YELLOW}No Linuxtrack installations found in standard prefixes.${NC}"
    else
        log "Detected installations:"
        printf '  %s\n' "${FOUND_INSTALLS[@]}"
    fi

    local config_file="$HOME/.config/linuxtrack/linuxtrack1.conf"
    if [ -f "$config_file" ]; then
        ACTIVE_PREFIX=$(grep -i '^Prefix' "$config_file" | cut -d'"' -f2 || true)
        if [ -n "$ACTIVE_PREFIX" ]; then
            log "Config prefix: $ACTIVE_PREFIX"
        fi
    else
        log "Config file: not created yet ($config_file)"
    fi

    if [ "${#FOUND_INSTALLS[@]}" -gt 0 ]; then
        local gui="${FOUND_INSTALLS[0]}/bin/ltr_gui"
        local missing
        missing=$(ldd "$gui" | grep "not found" || true)
        if [ -z "$missing" ]; then
            log "${GREEN}Binary linkage: OK${NC}"
        else
            log "${RED}Binary linkage issues:${NC}"
            printf '%s\n' "$missing"
        fi
    fi

    local firmware_dir="$HOME/.config/linuxtrack/tir_firmware"
    if [ -f "$firmware_dir/TIRViews.dll" ]; then
        log "${GREEN}TrackIR firmware: present${NC}"
    else
        log "TrackIR firmware: not installed"
    fi
}

ensure_build_tools() {
    if ! command -v cmake >/dev/null 2>&1; then
        log "${RED}cmake is required.${NC}"
        exit 1
    fi
}

perform_build() {
    ensure_build_tools
    install_dependencies

    mkdir -p "$BUILD_DIR"
    log "\n${GREEN}🛠️  Configuring build${NC}"
    log "Project root: $PROJECT_ROOT"
    log "Build dir: $BUILD_DIR"
    log "Prefix: $PREFIX"

    cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR" -DCMAKE_INSTALL_PREFIX="$PREFIX"

    log "\n${GREEN}🔨 Building${NC}"
    cmake --build "$BUILD_DIR" -j"$(nproc)"
}

copy_manifest() {
    local manifest="$BUILD_DIR/install_manifest.txt"
    local target_dir="$PREFIX/share/linuxtrack"
    if [ ! -f "$manifest" ]; then
        return 0
    fi

    local sudo_prefix=""
    if [ ! -w "$PREFIX" ] && [ ! -w "$(dirname "$PREFIX")" ]; then
        sudo_prefix="sudo"
    fi

    $sudo_prefix mkdir -p "$target_dir"
    $sudo_prefix cp "$manifest" "$target_dir/install_manifest.txt"
}

update_config_prefix() {
    local config_file="$HOME/.config/linuxtrack/linuxtrack1.conf"
    mkdir -p "$(dirname "$config_file")"
    if [ -f "$config_file" ]; then
        cp "$config_file" "${config_file}.bak"
        if grep -qi '^Prefix' "$config_file"; then
            sed -i "s|^[Pp]refix.*=.*|Prefix = \"$PREFIX/bin\"|" "$config_file"
        else
            printf '\n[Global]\nPrefix = "%s/bin"\n' "$PREFIX" >> "$config_file"
        fi
    fi
}

perform_install() {
    if [ ! -x "$BUILD_DIR/src/qt_gui/ltr_gui" ]; then
        perform_build
    fi

    log "\n${GREEN}🚀 Installing to $PREFIX${NC}"

    local install_cmd=(cmake --install "$BUILD_DIR" --prefix "$PREFIX")
    if [ ! -w "$PREFIX" ] && [ ! -w "$(dirname "$PREFIX")" ]; then
        sudo "${install_cmd[@]}"
    else
        "${install_cmd[@]}"
    fi

    copy_manifest
    update_config_prefix
}

setup_integration() {
    local install_root="$PREFIX/share/linuxtrack"
    local tir_rules="$install_root/99-TIR.rules"
    local mickey_rules="$install_root/99-Mickey.rules"
    local desktop_src="$PROJECT_ROOT/src/linuxtrack.desktop"
    local desktop_dir="$HOME/.local/share/applications"

    if [ ! -f "$tir_rules" ]; then
        tir_rules="$PROJECT_ROOT/src/99-TIR.rules"
    fi
    if [ ! -f "$mickey_rules" ]; then
        mickey_rules="$PROJECT_ROOT/src/99-Mickey.rules"
    fi

    log "\n${GREEN}🛠️  System integration${NC}"

    if [ -f "$tir_rules" ]; then
        sudo cp "$tir_rules" /etc/udev/rules.d/
        if [ "$WITH_MICKEY" = true ] && [ -f "$mickey_rules" ]; then
            sudo cp "$mickey_rules" /etc/udev/rules.d/
        fi
        sudo udevadm control --reload-rules
        sudo udevadm trigger
        log "${GREEN}udev rules installed.${NC}"
    else
        log "${YELLOW}TrackIR rules not found; skipping udev integration.${NC}"
    fi

    mkdir -p "$desktop_dir"
    if [ -f "$desktop_src" ]; then
        sed "s|^Exec=.*|Exec=$PREFIX/bin/ltr_gui|" "$desktop_src" > "$desktop_dir/linuxtrack.desktop"
        log "${GREEN}Desktop entry installed to $desktop_dir.${NC}"
    fi
}

choose_existing_prefix() {
    scan_installations
    if [ "${#FOUND_INSTALLS[@]}" -eq 0 ]; then
        return 1
    fi
    if [ "${#FOUND_INSTALLS[@]}" -eq 1 ] || [ "$ASSUME_YES" = true ]; then
        PREFIX="${FOUND_INSTALLS[0]}"
        return 0
    fi

    log "Detected installations:"
    local i=1
    local path
    for path in "${FOUND_INSTALLS[@]}"; do
        log "  $i) $path"
        i=$((i + 1))
    done
    read -r -p "Select installation to upgrade: " sel
    PREFIX="${FOUND_INSTALLS[$((sel - 1))]}"
}

while [ $# -gt 0 ]; do
    case "$1" in
        --mode)
            MODE="$2"
            shift 2
            ;;
        --prefix)
            PREFIX="$2"
            shift 2
            ;;
        --build-dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        --yes|-y)
            ASSUME_YES=true
            shift
            ;;
        --skip-deps)
            SKIP_DEPS=true
            shift
            ;;
        --with-mickey)
            WITH_MICKEY=true
            shift
            ;;
        --help|-h)
            usage
            exit 0
            ;;
        *)
            log "${RED}Unknown argument: $1${NC}"
            usage
            exit 1
            ;;
    esac
done

case "$MODE" in
    health)
        show_health
        ;;
    build)
        perform_build
        ;;
    install)
        perform_install
        ;;
    upgrade)
        if choose_existing_prefix; then
            perform_install
        else
            log "${YELLOW}No existing installation detected; using --prefix=$PREFIX${NC}"
            perform_install
        fi
        ;;
    integrate)
        setup_integration
        ;;
    full)
        perform_install
        setup_integration
        ;;
    *)
        log "${RED}Unsupported mode: $MODE${NC}"
        usage
        exit 1
        ;;
esac
