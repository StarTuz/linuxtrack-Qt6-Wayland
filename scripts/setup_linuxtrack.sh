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
PREFIX_SET=false
BUILD_DIR="$DEFAULT_BUILD_DIR"
ASSUME_YES=false
SKIP_DEPS=false
WITH_MICKEY=false

DISTRO_ID="unknown"
PKG_MANAGER=""

FOUND_INSTALLS=()
ACTIVE_PREFIX=""
ACTIVE_GUI=""
PATH_PREFIX=""
PATH_GUI=""

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
  scripts/setup_linuxtrack.sh --mode upgrade --prefix /opt/linuxtrack
  scripts/setup_linuxtrack.sh --mode full --yes
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
            printf '%s\n' cmake qt6-base qt6-tools libusb mxml liblo opencv nlohmann-json
            ;;
        apt)
            printf '%s\n' \
                cmake build-essential pkg-config \
                qt6-base-dev qt6-tools-dev qt6-tools-dev-tools qt6-l10n-tools \
                libmxml-dev libusb-1.0-0-dev liblo-dev libopencv-dev \
                zlib1g-dev libv4l-dev libgl1-mesa-dev libglu1-mesa-dev \
                libx11-dev libxkbcommon-dev nlohmann-json3-dev
            ;;
        dnf)
            printf '%s\n' \
                cmake gcc gcc-c++ pkgconf-pkg-config \
                qt6-qtbase-devel qt6-qttools-devel \
                mxml-devel libusb1-devel liblo-devel opencv-devel \
                zlib-devel libv4l-devel mesa-libGL-devel mesa-libGLU-devel \
                libX11-devel libxkbcommon-devel nlohmann-json-devel
            ;;
        zypper)
            printf '%s\n' \
                cmake gcc gcc-c++ pkgconf-pkg-config \
                libqt6-qtbase-devel libqt6-qttools-devel \
                mxml-devel libusb-1_0-devel liblo-devel opencv-devel \
                zlib-devel libv4l-devel Mesa-libGL-devel glu-devel \
                libX11-devel libxkbcommon-devel nlohmann_json-devel
            ;;
    esac
}

optional_dependency_note() {
    case "$PKG_MANAGER" in
        pacman)
            printf '%s\n' "wine, wine-stable, or wine-staging for Windows/TrackIR integration"
            ;;
        apt|dnf|zypper)
            printf '%s\n' "wine for Windows/TrackIR integration"
            ;;
    esac
}

xplane_dependency_note() {
    case "$PKG_MANAGER" in
        pacman)
            printf '%s\n' "xplane-sdk-devel (AUR) -- required to build xlinuxtrack9.so"
            ;;
        *)
            printf '%s\n' "X-Plane SDK headers (developer.x-plane.com) -- required to build xlinuxtrack9.so"
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
    local optional_note
    optional_note="$(optional_dependency_note || true)"
    if [ -n "$optional_note" ]; then
        log "Optional runtime packages:"
        printf '  %s\n' "$optional_note"
    fi
    local xplane_note
    xplane_note="$(xplane_dependency_note || true)"
    if [ -n "$xplane_note" ]; then
        log "Required for X-Plane support:"
        printf '  %s\n' "$xplane_note"
    fi

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

is_appimage_mount_prefix() {
    case "$1" in
        /tmp/.mount_*|/tmp/.mount_*/*)
            return 0
            ;;
        *)
            return 1
            ;;
    esac
}

is_system_prefix() {
    case "$1" in
        /usr|/usr/local|/usr/local/|/usr/)
            return 0
            ;;
    esac
    return 1
}

is_valid_install_prefix() {
    local path="$1"
    if [ -z "$path" ]; then
        return 1
    fi
    if is_appimage_mount_prefix "$path"; then
        return 1
    fi
    if is_system_prefix "$path"; then
        return 1
    fi
    return 0
}

normalize_prefix_root() {
    local raw="$1"
    raw="${raw%/}"
    case "$raw" in
        */bin)
            printf '%s\n' "${raw%/bin}"
            ;;
        *)
            printf '%s\n' "$raw"
            ;;
    esac
}

read_active_prefix() {
    ACTIVE_PREFIX=""
    ACTIVE_GUI=""
    local config_file="$HOME/.config/linuxtrack/linuxtrack1.conf"
    if [ ! -f "$config_file" ]; then
        return 0
    fi

    local raw_prefix
    raw_prefix=$(grep -i '^Prefix' "$config_file" | tail -n 1 | cut -d'"' -f2 || true)
    if [ -z "$raw_prefix" ]; then
        return 0
    fi

    ACTIVE_PREFIX="$(normalize_prefix_root "$raw_prefix")"
    if [ -x "$ACTIVE_PREFIX/bin/ltr_gui" ]; then
        ACTIVE_GUI="$ACTIVE_PREFIX/bin/ltr_gui"
    elif [ -x "$raw_prefix/ltr_gui" ]; then
        ACTIVE_GUI="$raw_prefix/ltr_gui"
    fi
}

read_path_prefix() {
    PATH_PREFIX=""
    PATH_GUI=""
    local path_gui
    path_gui="$(command -v ltr_gui || true)"
    if [ -z "$path_gui" ]; then
        return 0
    fi
    PATH_GUI="$(readlink -f "$path_gui" 2>/dev/null || printf '%s' "$path_gui")"
    local candidate
    candidate="$(normalize_prefix_root "$(dirname "$(dirname "$PATH_GUI")")")"
    if is_valid_install_prefix "$candidate"; then
        PATH_PREFIX="$candidate"
    fi
}

describe_binary() {
    local binary="$1"
    if [ ! -e "$binary" ]; then
        return 0
    fi
    printf '  path: %s\n' "$binary"
    printf '  modified: %s\n' "$(stat -c '%y' "$binary" 2>/dev/null || stat -f '%Sm' "$binary")"
}

scan_installations() {
    FOUND_INSTALLS=()
    read_active_prefix
    read_path_prefix
    local search_paths=("/opt/linuxtrack" "$HOME/.local")
    local path
    for path in "${search_paths[@]}"; do
        if [ -x "$path/bin/ltr_gui" ]; then
            add_unique_install "$path"
        fi
    done
    if [ -n "$ACTIVE_PREFIX" ] && [ -x "$ACTIVE_PREFIX/bin/ltr_gui" ]; then
        if is_valid_install_prefix "$ACTIVE_PREFIX"; then
            add_unique_install "$ACTIVE_PREFIX"
        fi
    fi
    if [ -n "$PATH_PREFIX" ] && [ -x "$PATH_PREFIX/bin/ltr_gui" ] && is_valid_install_prefix "$PATH_PREFIX"; then
        add_unique_install "$PATH_PREFIX"
    fi
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
    if [ ! -f "$config_file" ]; then
        log "Config file: not created yet ($config_file)"
    elif [ -n "$ACTIVE_PREFIX" ]; then
        log "Active config prefix: $ACTIVE_PREFIX"
        if ! is_valid_install_prefix "$ACTIVE_PREFIX"; then
            log "${YELLOW}Active prefix is an AppImage runtime mount and cannot be upgraded in place.${NC}"
        fi
        if [ -n "$ACTIVE_GUI" ]; then
            log "Configured GUI binary:"
            describe_binary "$ACTIVE_GUI"
        else
            log "${YELLOW}Configured prefix does not currently contain bin/ltr_gui.${NC}"
        fi
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

    if [ -n "$PATH_GUI" ]; then
        log "PATH ltr_gui:"
        describe_binary "$PATH_GUI"
        if [ -n "$PATH_PREFIX" ] && is_valid_install_prefix "$PATH_PREFIX"; then
            log "Source upgrades will use PATH prefix: $PATH_PREFIX"
        fi
    fi

    if [ -x "$BUILD_DIR/src/qt_gui/ltr_gui" ]; then
        log "Local build artifact:"
        describe_binary "$BUILD_DIR/src/qt_gui/ltr_gui"
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
    if [ -x "$BUILD_DIR/src/qt_gui/ltr_gui" ]; then
        log "\n${GREEN}Built GUI artifact:${NC}"
        describe_binary "$BUILD_DIR/src/qt_gui/ltr_gui"
    fi
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
    else
        cat > "$config_file" <<EOF
[Global]
Prefix = "$PREFIX/bin"
EOF
    fi
}

perform_install() {
    if ! is_valid_install_prefix "$PREFIX"; then
        log "${RED}Refusing to install to AppImage runtime mount: $PREFIX${NC}"
        log "Run with a persistent prefix instead, for example:"
        log "  $0 --mode $MODE --prefix /opt/linuxtrack"
        exit 1
    fi

    perform_build

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
    if [ -n "$ACTIVE_PREFIX" ] && [ -x "$ACTIVE_PREFIX/bin/ltr_gui" ] && is_valid_install_prefix "$ACTIVE_PREFIX"; then
        PREFIX="$ACTIVE_PREFIX"
        log "Using active configured prefix: $PREFIX"
        return 0
    elif [ -n "$ACTIVE_PREFIX" ] && ! is_valid_install_prefix "$ACTIVE_PREFIX"; then
        log "${YELLOW}Ignoring active AppImage runtime prefix: $ACTIVE_PREFIX${NC}"
    fi
    if [ -n "$PATH_PREFIX" ] && [ -x "$PATH_PREFIX/bin/ltr_gui" ] && is_valid_install_prefix "$PATH_PREFIX"; then
        PREFIX="$PATH_PREFIX"
        log "Using PATH ltr_gui prefix: $PREFIX"
        return 0
    fi
    if [ "${#FOUND_INSTALLS[@]}" -eq 1 ] || [ "$ASSUME_YES" = true ]; then
        PREFIX="${FOUND_INSTALLS[0]}"
        log "Using detected prefix: $PREFIX"
        return 0
    fi

    log "Detected installations:"
    local i=1
    local path
    for path in "${FOUND_INSTALLS[@]}"; do
        log "  $i) $path"
        i=$((i + 1))
    done
    read -r -p "Select installation to upgrade [1-${#FOUND_INSTALLS[@]}]: " sel
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
            PREFIX_SET=true
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
        if [ "$PREFIX_SET" = true ]; then
            perform_install
        elif choose_existing_prefix; then
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
