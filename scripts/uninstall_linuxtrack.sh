#!/bin/bash
# Linuxtrack uninstaller with manifest support.

set -euo pipefail

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

ASSUME_YES=false
PURGE_USER_DATA=false
PREFIXES=()

usage() {
    cat <<EOF
Usage: $(basename "$0") [options]

Options:
  --prefix PATH         Uninstall a specific prefix
  --purge-user-data     Remove ~/.config/linuxtrack as well
  --yes, -y             Non-interactive mode where possible
  --help, -h            Show this help

This script prefers manifest-based uninstall from:
  <prefix>/share/linuxtrack/install_manifest.txt
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

add_prefix() {
    local prefix="$1"
    local existing
    for existing in "${PREFIXES[@]}"; do
        if [ "$existing" = "$prefix" ]; then
            return
        fi
    done
    PREFIXES+=("$prefix")
}

discover_prefixes() {
    local search_paths=("/opt/linuxtrack" "/usr/local" "/usr" "$HOME/.local")
    local path
    for path in "${search_paths[@]}"; do
        if [ -f "$path/share/linuxtrack/install_manifest.txt" ] || [ -x "$path/bin/ltr_gui" ]; then
            add_prefix "$path"
        fi
    done
}

remove_manifest_install() {
    local prefix="$1"
    local manifest="$prefix/share/linuxtrack/install_manifest.txt"
    local sudo_prefix=""

    if [ ! -f "$manifest" ]; then
        return 1
    fi
    if [ ! -w "$prefix" ] && [ ! -w "$(dirname "$prefix")" ]; then
        sudo_prefix="sudo"
    fi

    log "\n${GREEN}Removing manifest-managed install at $prefix${NC}"
    while IFS= read -r installed_path; do
        [ -n "$installed_path" ] || continue
        if [ -e "$installed_path" ] || [ -L "$installed_path" ]; then
            $sudo_prefix rm -f "$installed_path"
        fi
    done < "$manifest"

    $sudo_prefix rm -f "$manifest"
    $sudo_prefix rmdir --ignore-fail-on-non-empty "$prefix/share/linuxtrack/help/ltr_gui" 2>/dev/null || true
    $sudo_prefix rmdir --ignore-fail-on-non-empty "$prefix/share/linuxtrack/help/mickey" 2>/dev/null || true
    $sudo_prefix rmdir --ignore-fail-on-non-empty "$prefix/share/linuxtrack/help" 2>/dev/null || true
    $sudo_prefix rmdir --ignore-fail-on-non-empty "$prefix/share/linuxtrack" 2>/dev/null || true
    $sudo_prefix rmdir --ignore-fail-on-non-empty "$prefix/lib/linuxtrack" 2>/dev/null || true
    return 0
}

remove_legacy_install() {
    local prefix="$1"
    local sudo_prefix=""

    if [ ! -w "$prefix" ] && [ ! -w "$(dirname "$prefix")" ]; then
        sudo_prefix="sudo"
    fi

    log "\n${YELLOW}Removing legacy install at $prefix${NC}"

    if [ "$prefix" = "/usr" ] || [ "$prefix" = "/usr/local" ]; then
        $sudo_prefix rm -f "$prefix/bin/ltr_gui" "$prefix/bin/ltr_server1" \
            "$prefix/bin/ltr_recenter" "$prefix/bin/ltr_pipe" "$prefix/bin/ltr_extractor" \
            "$prefix/bin/ltr_udp" "$prefix/bin/osc_server" "$prefix/bin/ltr_hotkeyd" \
            "$prefix/bin/ltr_hotkey_gui" "$prefix/bin/mickey"
        $sudo_prefix rm -rf "$prefix/lib/linuxtrack"
        $sudo_prefix rm -rf "$prefix/share/linuxtrack"
        $sudo_prefix rm -f "$prefix/share/applications/linuxtrack.desktop" \
            "$prefix/share/applications/linuxtrack-wii.desktop"
    else
        $sudo_prefix rm -rf "$prefix"
    fi
}

cleanup_integration() {
    local rules=(/etc/udev/rules.d/99-TIR.rules /etc/udev/rules.d/99-Mickey.rules)
    local rule

    for rule in "${rules[@]}"; do
        if [ -f "$rule" ]; then
            sudo rm -f "$rule"
        fi
    done
    sudo udevadm control --reload-rules || true

    rm -f "$HOME/.local/share/applications/linuxtrack.desktop"
    rm -rf "$HOME/.local/lib/linuxtrack"
    sudo rm -f /usr/bin/ltr_gui /usr/bin/ltr_server1 /usr/bin/ltr_recenter \
        /usr/bin/ltr_pipe /usr/bin/ltr_extractor /usr/bin/ltr_udp \
        /usr/bin/ltr_webcam_probe /usr/bin/osc_server /usr/bin/ltr_hotkeyd \
        /usr/bin/ltr_hotkey_gui /usr/bin/mickey 2>/dev/null || true
    sudo rm -f /usr/share/applications/linuxtrack.desktop \
        /usr/share/applications/linuxtrack-wii.desktop \
        /usr/share/metainfo/linuxtrack.metainfo.xml \
        /usr/share/pixmaps/linuxtrack.xpm \
        /usr/share/pixmaps/linuxtrack-wii.xpm \
        /usr/share/icons/hicolor/scalable/apps/linuxtrack.svg \
        /usr/share/icons/hicolor/scalable/apps/linuxtrack-wii.svg \
        /usr/share/icons/hicolor/48x48/apps/linuxtrack.png \
        /usr/share/icons/hicolor/48x48/apps/linuxtrack-wii.png \
        /usr/lib/udev/rules.d/99-TIR.rules \
        /usr/lib/udev/rules.d/99-Mickey.rules 2>/dev/null || true
}

while [ $# -gt 0 ]; do
    case "$1" in
        --prefix)
            add_prefix "$2"
            shift 2
            ;;
        --purge-user-data)
            PURGE_USER_DATA=true
            shift
            ;;
        --yes|-y)
            ASSUME_YES=true
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

if [ "${#PREFIXES[@]}" -eq 0 ]; then
    discover_prefixes
fi

if [ "${#PREFIXES[@]}" -eq 0 ]; then
    log "${YELLOW}No Linuxtrack installations detected.${NC}"
else
    log "Detected installations:"
    printf '  %s\n' "${PREFIXES[@]}"
fi

if [ "${#PREFIXES[@]}" -gt 0 ] && confirm "Remove the detected Linuxtrack installations?"; then
    for prefix in "${PREFIXES[@]}"; do
        if ! remove_manifest_install "$prefix"; then
            remove_legacy_install "$prefix"
        fi
    done
fi

cleanup_integration

if [ "$PURGE_USER_DATA" = true ]; then
    rm -rf "$HOME/.config/linuxtrack"
elif [ -d "$HOME/.config/linuxtrack" ] && confirm "Remove user profiles and firmware from ~/.config/linuxtrack?"; then
    rm -rf "$HOME/.config/linuxtrack"
fi

log "\n${GREEN}Linuxtrack uninstall complete.${NC}"
