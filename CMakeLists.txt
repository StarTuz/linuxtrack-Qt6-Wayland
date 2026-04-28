cmake_minimum_required(VERSION 3.16)
project(LinuxTrack VERSION 1.4.3 LANGUAGES C CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# Options
option(BUILD_GUI "Build Qt GUI" ON)
option(BUILD_MICKEY "Build Mickey mouse emulator" ON)
option(BUILD_WINE_BRIDGE "Build Wine bridge (requires winegcc)" ON)
option(BUILD_MAC_EXPERIMENTAL
       "Enable experimental macOS-specific build scaffolding"
       OFF)
if(APPLE AND BUILD_MAC_EXPERIMENTAL)
    if(BUILD_MICKEY)
        message(STATUS "Disabling Mickey for the experimental macOS build")
        set(BUILD_MICKEY OFF CACHE BOOL "Build Mickey mouse emulator" FORCE)
    endif()
    if(BUILD_WINE_BRIDGE)
        message(STATUS "Disabling Wine bridge for the experimental macOS build")
        set(BUILD_WINE_BRIDGE OFF CACHE BOOL "Build Wine bridge (requires winegcc)" FORCE)
    endif()
endif()
if(UNIX AND NOT APPLE)
    option(LINUXTRACK_INSTALL_STANDARD_INTEGRATION
           "Install desktop files, wrappers, AppStream metadata, and udev rules into standard system paths"
           ON)
else()
    set(LINUXTRACK_INSTALL_STANDARD_INTEGRATION OFF)
endif()

# Default installation prefix
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if(APPLE AND BUILD_MAC_EXPERIMENTAL)
        set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/stage_macos_experimental"
            CACHE PATH "Default install path" FORCE)
    else()
        set(CMAKE_INSTALL_PREFIX "/opt/linuxtrack" CACHE PATH "Default install path" FORCE)
    endif()
endif()

# Project version (legacy compatibility)
set(PACKAGE_VERSION "${PROJECT_VERSION}")

# Include modules for feature testing
include(CheckIncludeFile)
include(CheckFunctionExists)
include(CheckTypeSize)
include(GNUInstallDirs)
include(FindPkgConfig)

# Custom CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Dependency Checks
pkg_search_module(MXML REQUIRED mxml mxml4)
pkg_check_modules(LIBUSB10 libusb-1.0)
pkg_check_modules(LIBLO liblo)
pkg_check_modules(OPENCV opencv4)

find_library(LTR_LIBM m)
find_library(LTR_LIBDL dl)
find_library(LTR_LIBPTHREAD pthread)
find_library(LTR_LIBV4L2 v4l2)

# ONNX Runtime for neural face tracking
option(BUILD_NEURALNET_TRACKER "Build neural network webcam face tracker" ON)
if(BUILD_NEURALNET_TRACKER)
    find_package(ONNXRuntime QUIET)
    if(ONNXRuntime_FOUND)
        set(HAVE_ONNXRUNTIME TRUE)
        message(STATUS "ONNXRuntime found - neural webcam face tracking enabled")
    else()
        set(HAVE_ONNXRUNTIME FALSE)
        message(STATUS "ONNXRuntime not found - neural webcam face tracking disabled")
    endif()
else()
    set(HAVE_ONNXRUNTIME FALSE)
endif()

if(HAVE_ONNXRUNTIME)
    find_package(OpenMP QUIET)
endif()

# Qt Dependencies for GUI and Mickey
if(BUILD_GUI OR BUILD_MICKEY)
    find_package(Qt6 COMPONENTS Widgets Gui Network Help OpenGL OpenGLWidgets REQUIRED)
endif()

# X11 for Mickey
if(BUILD_MICKEY AND UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
endif()

# GLU for GUI
if(BUILD_GUI)
    find_package(OpenGL REQUIRED)
    if(APPLE)
        set(GLU_LIBRARY "-framework OpenGL")
    else()
        find_library(GLU_LIBRARY GLU)
    endif()
endif()

find_package(ZLIB REQUIRED)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# Paths for installation and configuration
if(APPLE AND BUILD_MAC_EXPERIMENTAL)
    set(Linuxtrack_DATA_DIR
        "${CMAKE_INSTALL_PREFIX}/Linuxtrack.app/Contents/Resources/linuxtrack")
else()
    set(Linuxtrack_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/linuxtrack")
endif()
set(Linuxtrack_HELP_DIR "${Linuxtrack_DATA_DIR}/help")
add_definitions(-DLTR_DATA_PATH="${Linuxtrack_DATA_DIR}")

if(LINUXTRACK_INSTALL_STANDARD_INTEGRATION)
    set(LINUXTRACK_SYSTEM_BINDIR "/usr/bin" CACHE PATH
        "Absolute path for system command shims")
    set(LINUXTRACK_SYSTEM_APPLICATIONS_DIR "/usr/share/applications" CACHE PATH
        "Absolute path for desktop entries")
    set(LINUXTRACK_SYSTEM_PIXMAPS_DIR "/usr/share/pixmaps" CACHE PATH
        "Absolute path for legacy pixmap icons")
    set(LINUXTRACK_SYSTEM_ICON_SCALABLE_DIR "/usr/share/icons/hicolor/scalable/apps" CACHE PATH
        "Absolute path for scalable application icons")
    set(LINUXTRACK_SYSTEM_ICON_48_DIR "/usr/share/icons/hicolor/48x48/apps" CACHE PATH
        "Absolute path for 48x48 application icons")
    set(LINUXTRACK_SYSTEM_METAINFODIR "/usr/share/metainfo" CACHE PATH
        "Absolute path for AppStream metadata")
    set(LINUXTRACK_SYSTEM_UDEV_RULES_DIR "/usr/lib/udev/rules.d" CACHE PATH
        "Absolute path for packaged udev rules")
else()
    set(LINUXTRACK_SYSTEM_BINDIR "${CMAKE_INSTALL_BINDIR}")
    set(LINUXTRACK_SYSTEM_APPLICATIONS_DIR "${CMAKE_INSTALL_DATAROOTDIR}/applications")
    set(LINUXTRACK_SYSTEM_PIXMAPS_DIR "${CMAKE_INSTALL_DATAROOTDIR}/pixmaps")
    set(LINUXTRACK_SYSTEM_ICON_SCALABLE_DIR
        "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps")
    set(LINUXTRACK_SYSTEM_ICON_48_DIR
        "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/48x48/apps")
    set(LINUXTRACK_SYSTEM_METAINFODIR "${CMAKE_INSTALL_DATAROOTDIR}/metainfo")
    set(LINUXTRACK_SYSTEM_UDEV_RULES_DIR "${CMAKE_INSTALL_DATAROOTDIR}/linuxtrack")
endif()

# System Check: Headers
check_include_file(assert.h HAVE_ASSERT_H)
check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(netdb.h HAVE_NETDB_H)
check_include_file(stdbool.h HAVE_STDBOOL_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdio.h HAVE_STDIO_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(sys/ioctl.h HAVE_SYS_IOCTL_H)
check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(unistd.h HAVE_UNISTD_H)

# System Check: Functions
check_function_exists(atexit HAVE_ATEXIT)
check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists(memset HAVE_MEMSET)
check_function_exists(select HAVE_SELECT)
check_function_exists(socket HAVE_SOCKET)
check_function_exists(strerror HAVE_STRERROR)

# Platform specifics
if(APPLE)
    set(HAS_DARWIN 1)
    add_definitions(-DAPL=1 -DIBM=0 -DLIN=0 -DDARWIN)
    if(BUILD_MAC_EXPERIMENTAL)
        message(STATUS "Experimental macOS build scaffolding enabled")
    else()
        message(STATUS "macOS experimental scaffolding disabled; Linuxtrack remains Linux-first")
    endif()
else()
    set(HAS_LINUX 1)
    add_definitions(-DAPL=0 -DIBM=0 -DLIN=1)
    set(HAS_V4L2 1)
endif()

# Configuration variables for config.h
set(PACKAGE_BUGREPORT "https://github.com/uglyDwarf/linuxtrack/issues")
set(PACKAGE_NAME "LinuxTrack")
set(PACKAGE_TARNAME "linuxtrack")
set(PACKAGE_URL "")

# Generate headers
configure_file(config.h.cmake ${CMAKE_BINARY_DIR}/config.h)
configure_file(src/pathconfig.h.cmake ${CMAKE_BINARY_DIR}/src/pathconfig.h)

# Global compiler settings
add_definitions(-DHAVE_CONFIG_H)
if(NOT APPLE)
    add_definitions(-DLIB_PATH="${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/")
endif()

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/src)

# RPATH settings for installation
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
if(APPLE)
    set(CMAKE_INSTALL_RPATH
        "@loader_path/../Frameworks;@loader_path/../lib/linuxtrack;@loader_path/../lib")
else()
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack:${CMAKE_INSTALL_PREFIX}/lib")

    # Wait, we want $ORIGIN based RPATH for relocatability sometimes,
    # but for /opt/linuxtrack absolute paths are fine too since that's the fixed prefix.
    # However, to be modern:
    file(RELATIVE_PATH REL_LIB_PATH "${CMAKE_INSTALL_PREFIX}/bin" "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack")
    list(APPEND CMAKE_INSTALL_RPATH "$ORIGIN/${REL_LIB_PATH}")
    file(RELATIVE_PATH REL_LIB_PATH2 "${CMAKE_INSTALL_PREFIX}/bin" "${CMAKE_INSTALL_PREFIX}/lib")
    list(APPEND CMAKE_INSTALL_RPATH "$ORIGIN/${REL_LIB_PATH2}")
endif()

# Enable CTest support
include(CTest)

function(linuxtrack_install_command_shim name target_path)
    install(CODE "
        set(_dest \"\$ENV{DESTDIR}${LINUXTRACK_SYSTEM_BINDIR}\")
        file(MAKE_DIRECTORY \"\${_dest}\")
        execute_process(COMMAND \"${CMAKE_COMMAND}\" -E rm -f \"\${_dest}/${name}\")
        execute_process(COMMAND \"${CMAKE_COMMAND}\" -E create_symlink
                        \"${target_path}\" \"\${_dest}/${name}\")
    ")
endfunction()

# Subdirectories
add_subdirectory(src)

if(BUILD_GUI)
    add_subdirectory(src/qt_gui)
endif()

if(BUILD_MICKEY)
    add_subdirectory(src/mickey)
endif()

if(BUILD_WINE_BRIDGE)
    find_program(WINEGCC winegcc)
    find_program(WINEGXX wineg++)
    find_program(WRC wrc)
    find_program(MAKENSIS makensis)
    if(WINEGCC AND WINEGXX AND WRC)
        message(STATUS "Found Wine build tools - building Wine bridge")
        add_subdirectory(src/wine_bridge)
    else()
        message(WARNING "Wine build tools not found - skipping Wine bridge")
    endif()
endif()

# Installation of global files
install(FILES README README.devel README.xplane doc/README.ltr_pipe DESTINATION share/doc/linuxtrack)
if(NOT APPLE)
    install(FILES src/linuxtrack.desktop src/linuxtrack-wii.desktop
            DESTINATION "${LINUXTRACK_SYSTEM_APPLICATIONS_DIR}")
    install(FILES src/linuxtrack.svg src/linuxtrack-wii.svg
            DESTINATION "${LINUXTRACK_SYSTEM_ICON_SCALABLE_DIR}")
    install(FILES src/linuxtrack.png src/linuxtrack-wii.png
            DESTINATION "${LINUXTRACK_SYSTEM_ICON_48_DIR}")
    install(FILES src/linuxtrack.xpm src/linuxtrack-wii.xpm
            DESTINATION "${LINUXTRACK_SYSTEM_PIXMAPS_DIR}")
    install(FILES packaging/linuxtrack.metainfo.xml
            DESTINATION "${LINUXTRACK_SYSTEM_METAINFODIR}")
endif()
if(LINUXTRACK_INSTALL_STANDARD_INTEGRATION)
    install(FILES src/99-TIR.rules src/99-Mickey.rules
            DESTINATION "${LINUXTRACK_SYSTEM_UDEV_RULES_DIR}")
endif()

if(HAVE_ONNXRUNTIME)
    install(FILES
        opentrack_research/tracker-neuralnet/models/head-localizer.onnx
        opentrack_research/tracker-neuralnet/models/head-pose-0.4-big-int8.onnx
        opentrack_research/tracker-neuralnet/models/head-pose-0.4-small-f32.onnx
        DESTINATION share/linuxtrack/models)
endif()

if(LINUXTRACK_INSTALL_STANDARD_INTEGRATION AND NOT CMAKE_INSTALL_PREFIX STREQUAL "/usr")
    if(TARGET ltr_gui)
        linuxtrack_install_command_shim(ltr_gui "${CMAKE_INSTALL_PREFIX}/bin/ltr_gui")
    endif()
    if(TARGET ltr_server1)
        linuxtrack_install_command_shim(ltr_server1 "${CMAKE_INSTALL_PREFIX}/bin/ltr_server1")
    endif()
    if(TARGET ltr_recenter)
        linuxtrack_install_command_shim(ltr_recenter "${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter")
    endif()
    if(TARGET ltr_pipe)
        linuxtrack_install_command_shim(ltr_pipe "${CMAKE_INSTALL_PREFIX}/bin/ltr_pipe")
    endif()
    if(TARGET ltr_extractor)
        linuxtrack_install_command_shim(ltr_extractor "${CMAKE_INSTALL_PREFIX}/bin/ltr_extractor")
    endif()
    if(TARGET ltr_udp)
        linuxtrack_install_command_shim(ltr_udp "${CMAKE_INSTALL_PREFIX}/bin/ltr_udp")
    endif()
    if(TARGET ltr_webcam_probe)
        linuxtrack_install_command_shim(ltr_webcam_probe "${CMAKE_INSTALL_PREFIX}/bin/ltr_webcam_probe")
    endif()
    if(TARGET osc_server)
        linuxtrack_install_command_shim(osc_server "${CMAKE_INSTALL_PREFIX}/bin/osc_server")
    endif()
    if(TARGET ltr_hotkeyd)
        linuxtrack_install_command_shim(ltr_hotkeyd "${CMAKE_INSTALL_PREFIX}/bin/ltr_hotkeyd")
    endif()
    if(TARGET ltr_hotkey_gui)
        linuxtrack_install_command_shim(ltr_hotkey_gui "${CMAKE_INSTALL_PREFIX}/bin/ltr_hotkey_gui")
    endif()
    if(TARGET mickey)
        linuxtrack_install_command_shim(mickey "${CMAKE_INSTALL_PREFIX}/bin/mickey")
    endif()
endif()

if(UNIX AND NOT APPLE)
    include(cmake/LinuxtrackCPack.cmake)
endif()
