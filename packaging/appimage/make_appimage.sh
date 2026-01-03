#!/bin/bash
set -e

# Configuration
APP_NAME="linuxtrack"
BUILD_DIR="${BUILD_DIR:-build}"
APP_DIR="AppDir"

# Ensure we are in the project root
if [ ! -f "src/CMakeLists.txt" ]; then
    echo "Error: Please run this script from the project root directory."
    exit 1
fi

# Clean up previous artifacts
rm -rf "$APP_DIR"
rm -f *.AppImage

# 1. Install project to AppDir
echo "--> Installing to $APP_DIR..."
DESTDIR=$(pwd)/"$APP_DIR" cmake --install "$BUILD_DIR" --prefix "/usr"

# 2. Setup AppImage resources
echo "--> Setting up AppImage resources..."

# If dist32 exists (from a 2-pass CI build), copy those libs into the AppDir
if [ -d "dist32" ]; then
    echo "--> Found 32-bit libraries in dist32, copying to AppDir..."
    mkdir -p "$APP_DIR/usr/lib/linuxtrack/"
    cp -v dist32/*.so "$APP_DIR/usr/lib/linuxtrack/" 2>/dev/null || true
fi

# Copy icon and desktop file to standard request locations if not already there managed by install
# (Our install step usually puts them in /usr/share/applications and /usr/share/pixmaps)

# Verify desktop file exists in AppDir
if [ ! -f "$APP_DIR/usr/share/applications/$APP_NAME.desktop" ]; then
    echo "Warning: Desktop file not found in AppDir at standard location."
    # Attempt to copy manually if missing
    mkdir -p "$APP_DIR/usr/share/applications"
    cp src/$APP_NAME.desktop "$APP_DIR/usr/share/applications/"
fi

# Verify icon exists in AppDir
if [ ! -f "$APP_DIR/usr/share/icons/hicolor/scalable/apps/$APP_NAME.svg" ] && [ ! -f "$APP_DIR/usr/share/pixmaps/$APP_NAME.svg" ]; then
    echo "Warning: Icon file not found in AppDir at standard location."
    # Attempt to copy manually if missing
    mkdir -p "$APP_DIR/usr/share/icons/hicolor/scalable/apps"
    cp src/$APP_NAME.svg "$APP_DIR/usr/share/icons/hicolor/scalable/apps/"
fi

# Copy custom AppRun script that sets environment before Qt loads
echo "--> Installing custom AppRun script..."
cp packaging/appimage/AppRun "$APP_DIR/AppRun"
chmod +x "$APP_DIR/AppRun"

# 3. Download LinuxDeploy tools (if not present)
TOOLS_DIR="packaging/tools"
mkdir -p "$TOOLS_DIR"

LINUXDEPLOY="$TOOLS_DIR/linuxdeploy-x86_64.AppImage"
LINUXDEPLOY_PLUGIN_QT="$TOOLS_DIR/linuxdeploy-plugin-qt-x86_64.AppImage"

if [ ! -f "$LINUXDEPLOY" ]; then
    echo "--> Downloading linuxdeploy..."
    wget -q -O "$LINUXDEPLOY" "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"
    chmod +x "$LINUXDEPLOY"
fi

if [ ! -f "$LINUXDEPLOY_PLUGIN_QT" ]; then
    echo "--> Downloading linuxdeploy-plugin-qt..."
    wget -q -O "$LINUXDEPLOY_PLUGIN_QT" "https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage"
    chmod +x "$LINUXDEPLOY_PLUGIN_QT"
fi

# 4. Pre-copy Qt plugins for Arch Linux compatibility
# linuxdeploy-plugin-qt sometimes fails to find plugins on Arch, so we help it
echo "--> Pre-copying Qt plugins for Arch compatibility..."
QT_PLUGIN_SRC=$(qmake6 -query QT_INSTALL_PLUGINS 2>/dev/null || echo "/usr/lib/qt6/plugins")
if [ -d "$QT_PLUGIN_SRC" ]; then
    # Copy to BOTH locations - AppRun uses /usr/plugins, linuxdeploy expects /usr/lib/qt6/plugins
    mkdir -p "$APP_DIR/usr/plugins"
    mkdir -p "$APP_DIR/usr/lib/qt6/plugins"
    
    # Copy ONLY the essential platform plugin (xcb)
    if [ -d "$QT_PLUGIN_SRC/platforms" ]; then
        mkdir -p "$APP_DIR/usr/plugins/platforms"
        mkdir -p "$APP_DIR/usr/lib/qt6/plugins/platforms"
        # Only copy xcb and minimal platforms, skip wayland/others that add deps
        for plugin in libqxcb.so libqminimal.so libqoffscreen.so; do
            if [ -f "$QT_PLUGIN_SRC/platforms/$plugin" ]; then
                cp "$QT_PLUGIN_SRC/platforms/$plugin" "$APP_DIR/usr/plugins/platforms/"
                cp "$QT_PLUGIN_SRC/platforms/$plugin" "$APP_DIR/usr/lib/qt6/plugins/platforms/"
            fi
        done
    fi
    
    # Copy xcbglintegrations if present (needed for OpenGL)
    if [ -d "$QT_PLUGIN_SRC/xcbglintegrations" ]; then
        cp -r "$QT_PLUGIN_SRC/xcbglintegrations" "$APP_DIR/usr/plugins/"
        cp -r "$QT_PLUGIN_SRC/xcbglintegrations" "$APP_DIR/usr/lib/qt6/plugins/"
    fi
    
    # Copy only essential imageformats (skip KDE kimg_* plugins that have optional deps)
    if [ -d "$QT_PLUGIN_SRC/imageformats" ]; then
        mkdir -p "$APP_DIR/usr/plugins/imageformats"
        mkdir -p "$APP_DIR/usr/lib/qt6/plugins/imageformats"
        # Only copy Qt's own image plugins, not KDE's kimageformats
        for plugin in libqgif.so libqico.so libqjpeg.so libqsvg.so; do
            if [ -f "$QT_PLUGIN_SRC/imageformats/$plugin" ]; then
                cp "$QT_PLUGIN_SRC/imageformats/$plugin" "$APP_DIR/usr/plugins/imageformats/"
                cp "$QT_PLUGIN_SRC/imageformats/$plugin" "$APP_DIR/usr/lib/qt6/plugins/imageformats/"
            fi
        done
    fi
    
    # Copy iconengines (for SVG icons)
    if [ -d "$QT_PLUGIN_SRC/iconengines" ]; then
        mkdir -p "$APP_DIR/usr/plugins/iconengines"
        mkdir -p "$APP_DIR/usr/lib/qt6/plugins/iconengines"
        if [ -f "$QT_PLUGIN_SRC/iconengines/libqsvgicon.so" ]; then
            cp "$QT_PLUGIN_SRC/iconengines/libqsvgicon.so" "$APP_DIR/usr/plugins/iconengines/"
            cp "$QT_PLUGIN_SRC/iconengines/libqsvgicon.so" "$APP_DIR/usr/lib/qt6/plugins/iconengines/"
        fi
    fi
    
    echo "    Copied essential Qt plugins from $QT_PLUGIN_SRC"
else
    echo "Warning: Qt plugin source not found at $QT_PLUGIN_SRC"
fi

# 5. Generate AppImage
echo "--> Generating AppImage..."

# We need to set QMAKE path for the plugin to find Qt6
# Try to find qmake6, qmake in common locations
export QMAKE=$(which qmake6 || which qmake || find /usr/lib/qt6/bin -name qmake 2>/dev/null | head -n 1)
if [ -z "$QMAKE" ]; then
    echo "Warning: qmake not found. Attempting to use default Qt plugin detection."
else
    echo "Using qmake: $QMAKE"
fi

# Run linuxdeploy
# --appdir: target AppDir
# NOTE: We skip --plugin qt because it has issues on Arch Linux
# Qt plugins are pre-copied above and dependencies are bundled by linuxdeploy core
# --output appimage: create the actual file
# Note: we use LD_LIBRARY_PATH to help linuxdeploy find our internal libraries
export LD_LIBRARY_PATH="$(pwd)/$APP_DIR/usr/lib:$(pwd)/$APP_DIR/usr/lib/linuxtrack:$LD_LIBRARY_PATH"

# We must set ARCH because we bundle both 32-bit and 64-bit libraries
export ARCH=x86_64

# CRITICAL: Skip strip on Arch Linux - the bundled strip in linuxdeploy 
# doesn't understand newer ELF .relr.dyn sections used by Arch's toolchain.
# This results in larger AppImage but avoids build failures.
export NO_STRIP=1

# Run linuxdeploy WITHOUT the Qt plugin - we handle Qt ourselves
"$LINUXDEPLOY" --appdir "$APP_DIR" \
    --desktop-file "$APP_DIR/usr/share/applications/$APP_NAME.desktop" \
    --icon-file "$APP_DIR/usr/share/pixmaps/$APP_NAME.svg" \
    --output appimage

echo "--> Done! AppImage created."
ls -lh *.AppImage
