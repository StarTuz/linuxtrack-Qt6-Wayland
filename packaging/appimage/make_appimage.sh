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

# Needed for running linuxdeploy (an AppImage) inside Docker/non-FUSE environments
export APPIMAGE_EXTRACT_AND_RUN=1

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

cp packaging/appimage/AppRun "$APP_DIR/AppRun"
chmod +x "$APP_DIR/AppRun"

# 2b. Ensure Model Data is present
echo "--> Verifying model data in AppDir..."
mkdir -p "$APP_DIR/usr/share/linuxtrack"
# Copy from source just in case install missed them or used wrong path
cp -v src/qt_gui/*.obj "$APP_DIR/usr/share/linuxtrack/" || true
cp -v src/qt_gui/sphere.png src/qt_gui/xm8_detail.png "$APP_DIR/usr/share/linuxtrack/" || true

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

# 4. Generate AppImage
echo "--> Generating AppImage..."

# Force Qt6
export QMAKE=$(which qmake6 || find /usr/lib/qt6/bin -name qmake 2>/dev/null | head -n 1 || which qmake)
if [[ "$QMAKE" != *"6"* ]] && [ -x "/usr/bin/qmake6" ]; then
    QMAKE="/usr/bin/qmake6"
fi

if [ -z "$QMAKE" ]; then
    echo "Error: qmake (Qt6) not found. Cannot proceed with AppImage bundling."
    exit 1
fi
echo "Using qmake: $QMAKE"

# Run linuxdeploy
export LD_LIBRARY_PATH="$(pwd)/$APP_DIR/usr/lib/linuxtrack:$LD_LIBRARY_PATH"
export ARCH=x86_64

# Set Qt environment for proper plugin discovery
# We explicitly list common problematic plugins to ensure they are picked up
export EXTRA_QT_PLUGINS="platforms/libqxcb.so;platforms/libqwayland.so;platformthemes/libqgtk3.so;iconengines;xcbglintegrations;imageformats;wayland-graphics-integration-client"

export STRIP=${STRIP:-true}

# Avoid SQL drivers which are the most common source of "libfbclient.so.2 not found"
export QT_INSTALL_SQLDRIVERS=""

# Find all libraries in our specific subdir to ensure linuxdeploy bundles them
LIBRARY_FLAGS=""
for lib in "$APP_DIR"/usr/lib/linuxtrack/*.so*; do
    if [ -f "$lib" ]; then
        LIBRARY_FLAGS="$LIBRARY_FLAGS --library $lib"
    fi
done

# Try linuxdeploy first (bundles dependencies and Qt plugins)
# It may fail on strip step due to modern ELF .relr.dyn sections on Arch/Fedora
echo "--> Running linuxdeploy to bundle dependencies..."
set +e  # Don't exit on error - strip failures are recoverable
"$LINUXDEPLOY" --appdir "$APP_DIR" \
    --desktop-file "$APP_DIR/usr/share/applications/$APP_NAME.desktop" \
    --icon-file "$APP_DIR/usr/share/pixmaps/$APP_NAME.svg" \
    $LIBRARY_FLAGS \
    --exclude-library libfbclient.so.2 \
    --exclude-library liboci.so \
    --exclude-library libqsqlibase.so \
    --exclude-library libqsqloci.so \
    --exclude-library libqsqlpsql.so \
    --exclude-library libqsqlodbc.so \
    --exclude-library libqsqlmysql.so \
    --plugin qt \
    --output appimage
LINUXDEPLOY_EXIT=$?
set -e

# 5. Cleanup (Post-bundling)
# Remove libraries that MUST NOT be bundled to avoid driver conflicts
echo "--> Purging system graphics libraries from AppDir to ensure host driver usage..."
find "$APP_DIR" -name "libGL.so*" -delete 2>/dev/null || true
find "$APP_DIR" -name "libEGL.so*" -delete 2>/dev/null || true
find "$APP_DIR" -name "libgbm.so*" -delete 2>/dev/null || true
find "$APP_DIR" -name "libdrm.so*" -delete 2>/dev/null || true
find "$APP_DIR" -name "libxcb-dri*" -delete 2>/dev/null || true
find "$APP_DIR" -name "libvulkan.so*" -delete 2>/dev/null || true

# Check if linuxdeploy created an AppImage
if ls *.AppImage 1>/dev/null 2>&1; then
    echo "--> linuxdeploy succeeded!"
    ls -lh *.AppImage
    exit 0
fi

# If linuxdeploy failed (usually due to strip errors on modern distros),
# use appimagetool directly to package the AppDir
echo "--> linuxdeploy failed (likely strip errors), falling back to appimagetool..."

# Setup AppDir for appimagetool
cp "$APP_DIR/usr/share/applications/$APP_NAME.desktop" "$APP_DIR/"
cp "$APP_DIR/usr/share/pixmaps/$APP_NAME.svg" "$APP_DIR/"
ln -sf "$APP_NAME.svg" "$APP_DIR/.DirIcon"

# 4b. Explicitly bundle Qt6 platform plugins (CRITICAL fallback for modern distros)
# linuxdeploy-plugin-qt often fails to bundle these when strip fails
echo "--> Manually bundling critical Qt6 plugins..."
QT_PLUGINS_DIR=$($QMAKE -query QT_INSTALL_PLUGINS)
if [ -d "$QT_PLUGINS_DIR" ]; then
    mkdir -p "$APP_DIR/usr/plugins/platforms"
    mkdir -p "$APP_DIR/usr/plugins/xcbglintegrations"
    mkdir -p "$APP_DIR/usr/plugins/wayland-graphics-integration-client"
    
    echo "    Copying from $QT_PLUGINS_DIR..."
    cp -v "$QT_PLUGINS_DIR/platforms/libqxcb.so" "$APP_DIR/usr/plugins/platforms/" 2>/dev/null || true
    cp -v "$QT_PLUGINS_DIR/platforms/libqwayland-egl.so" "$APP_DIR/usr/plugins/platforms/" 2>/dev/null || true
    cp -v "$QT_PLUGINS_DIR/platforms/libqwayland-generic.so" "$APP_DIR/usr/plugins/platforms/" 2>/dev/null || true
    cp -v "$QT_PLUGINS_DIR/xcbglintegrations/"*.so "$APP_DIR/usr/plugins/xcbglintegrations/" 2>/dev/null || true
    cp -v "$QT_PLUGINS_DIR/wayland-graphics-integration-client/"*.so "$APP_DIR/usr/plugins/wayland-graphics-integration-client/" 2>/dev/null || true
else
    echo "Warning: Could not locate Qt6 plugins directory via qmake."
fi

# Get appimagetool if needed
APPIMAGETOOL=$(which appimagetool 2>/dev/null || echo "")
if [ -z "$APPIMAGETOOL" ]; then
    echo "--> Downloading appimagetool..."
    APPIMAGETOOL="$TOOLS_DIR/appimagetool-x86_64.AppImage"
    if [ ! -f "$APPIMAGETOOL" ]; then
        wget -q -O "$APPIMAGETOOL" "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
        chmod +x "$APPIMAGETOOL"
    fi
fi

echo "--> Building AppImage with appimagetool..."
ARCH=x86_64 "$APPIMAGETOOL" "$APP_DIR" "Linuxtrack-x86_64.AppImage"

echo "--> Done! AppImage created."
ls -lh *.AppImage
