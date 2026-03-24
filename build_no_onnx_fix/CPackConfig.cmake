# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BUILD_SOURCE_DIRS "/home/startux/Code/linuxtrackfixed/linuxtrack;/home/startux/Code/linuxtrackfixed/linuxtrack/build_no_onnx_fix")
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "/home/startux/Code/linuxtrackfixed/linuxtrack/packaging/cpack/postinst.sh;/home/startux/Code/linuxtrackfixed/linuxtrack/packaging/cpack/postrm.sh")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Linuxtrack is a head-tracking suite for Linux flight simulators and games.

This package installs the full application payload under /opt/linuxtrack and
adds system integration files under standard system paths:

- /usr/bin command shims for user-facing tools
- /usr/share/applications desktop entries
- /usr/share/icons and /usr/share/pixmaps icons
- /usr/share/metainfo AppStream metadata
- /usr/lib/udev/rules.d packaged udev rules

Linuxtrack supports TrackIR devices, webcam blob tracking, and webcam face
tracking, with native Linux integrations plus Wine/Proton compatibility
helpers for Windows titles.
")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Matt Johnson")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS "ON")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS_PRIVATE_DIRS "/home/startux/Code/linuxtrackfixed/linuxtrack/build_no_onnx_fix/src;/home/startux/Code/linuxtrackfixed/linuxtrack/build_no_onnx_fix/src/qt_gui;/home/startux/Code/linuxtrackfixed/linuxtrack/build_no_onnx_fix/src/mickey;/home/startux/Code/linuxtrackfixed/linuxtrack/build_no_onnx_fix/src/ltr_hotkey_gui;/home/startux/Code/linuxtrackfixed/linuxtrack/build_no_onnx_fix/src/npclient_linux;/home/startux/Code/linuxtrackfixed/linuxtrack/build_no_onnx_fix/src/scs_plugin")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "LinuxTrack built using CMake")
set(CPACK_DMG_SLA_USE_RESOURCE_FILE_LICENSE "ON")
set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY "OFF")
set(CPACK_INNOSETUP_ARCHITECTURE "x64")
set(CPACK_INSTALL_CMAKE_PROJECTS "/home/startux/Code/linuxtrackfixed/linuxtrack/build_no_onnx_fix;LinuxTrack;ALL;/")
set(CPACK_INSTALL_PREFIX "/opt/linuxtrack")
set(CPACK_MODULE_PATH "/home/startux/Code/linuxtrackfixed/linuxtrack/cmake;/usr/lib/cmake/Qt6;/usr/lib/cmake/Qt6/3rdparty/extra-cmake-modules/find-modules;/usr/lib/cmake/Qt6/3rdparty/kwin")
set(CPACK_NSIS_DISPLAY_NAME "linuxtrack")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
set(CPACK_NSIS_PACKAGE_NAME "linuxtrack")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OBJCOPY_EXECUTABLE "/usr/bin/objcopy")
set(CPACK_OBJDUMP_EXECUTABLE "/usr/bin/objdump")
set(CPACK_OUTPUT_CONFIG_FILE "/home/startux/Code/linuxtrackfixed/linuxtrack/build_no_onnx_fix/CPackConfig.cmake")
set(CPACK_PACKAGE_CONTACT "Matt Johnson <matt.g.johnson101@gmail.com>")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "/home/startux/Code/linuxtrackfixed/linuxtrack/packaging/package_description.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Head tracking for Linux with TrackIR, webcam, and face tracking support")
set(CPACK_PACKAGE_FILE_NAME "linuxtrack-1.3.7-Linux-x86_64")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/StarTuz/linuxtrack")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "linuxtrack")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "linuxtrack")
set(CPACK_PACKAGE_NAME "linuxtrack")
set(CPACK_PACKAGE_RELOCATABLE "false")
set(CPACK_PACKAGE_VENDOR "Linuxtrack contributors")
set(CPACK_PACKAGE_VERSION "1.3.7")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "3")
set(CPACK_PACKAGE_VERSION_PATCH "7")
set(CPACK_PACKAGING_INSTALL_PREFIX "/")
set(CPACK_READELF_EXECUTABLE "/usr/bin/readelf")
set(CPACK_RESOURCE_FILE_LICENSE "/home/startux/Code/linuxtrackfixed/linuxtrack/LICENSE.md")
set(CPACK_RESOURCE_FILE_README "/home/startux/Code/linuxtrackfixed/linuxtrack/README.md")
set(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake/Templates/CPack.GenericWelcome.txt")
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/opt;/opt/linuxtrack;/usr/bin;/usr/lib;/usr/lib/udev;/usr/lib/udev/rules.d;/usr/share;/usr/share/applications;/usr/share/icons;/usr/share/icons/hicolor;/usr/share/icons/hicolor/48x48;/usr/share/icons/hicolor/48x48/apps;/usr/share/icons/hicolor/scalable;/usr/share/icons/hicolor/scalable/apps;/usr/share/metainfo;/usr/share/pixmaps")
set(CPACK_RPM_FILE_NAME "RPM-DEFAULT")
set(CPACK_RPM_PACKAGE_AUTOREQPROV "ON")
set(CPACK_RPM_PACKAGE_DESCRIPTION "Linuxtrack is a head-tracking suite for Linux flight simulators and games.

This package installs the full application payload under /opt/linuxtrack and
adds system integration files under standard system paths:

- /usr/bin command shims for user-facing tools
- /usr/share/applications desktop entries
- /usr/share/icons and /usr/share/pixmaps icons
- /usr/share/metainfo AppStream metadata
- /usr/lib/udev/rules.d packaged udev rules

Linuxtrack supports TrackIR devices, webcam blob tracking, and webcam face
tracking, with native Linux integrations plus Wine/Proton compatibility
helpers for Windows titles.
")
set(CPACK_RPM_PACKAGE_GROUP "Amusements/Games")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_URL "https://github.com/StarTuz/linuxtrack")
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "/home/startux/Code/linuxtrackfixed/linuxtrack/packaging/cpack/postinst.sh")
set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE "/home/startux/Code/linuxtrackfixed/linuxtrack/packaging/cpack/postrm.sh")
set(CPACK_SET_DESTDIR "ON")
set(CPACK_SOURCE_GENERATOR "TBZ2;TGZ;TXZ;TZ")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home/startux/Code/linuxtrackfixed/linuxtrack/build_no_onnx_fix/CPackSourceConfig.cmake")
set(CPACK_SOURCE_RPM "OFF")
set(CPACK_SOURCE_TBZ2 "ON")
set(CPACK_SOURCE_TGZ "ON")
set(CPACK_SOURCE_TXZ "ON")
set(CPACK_SOURCE_TZ "ON")
set(CPACK_SOURCE_ZIP "OFF")
set(CPACK_STRIP_FILES "OFF")
set(CPACK_SYSTEM_NAME "Linux")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "Linux")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/home/startux/Code/linuxtrackfixed/linuxtrack/build_no_onnx_fix/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
