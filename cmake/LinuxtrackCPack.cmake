set(CPACK_PACKAGE_NAME "linuxtrack")
set(CPACK_PACKAGE_VENDOR "Linuxtrack contributors")
set(CPACK_PACKAGE_CONTACT "Matt Johnson <matt.g.johnson101@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Head tracking for Linux with TrackIR, webcam, and face tracking support")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/StarTuz/linuxtrack")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "linuxtrack")
set(CPACK_PACKAGE_FILE_NAME
    "linuxtrack-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/packaging/package_description.txt")
file(READ "${CMAKE_SOURCE_DIR}/packaging/package_description.txt"
     LINUXTRACK_PACKAGE_DESCRIPTION)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.md")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_STRIP_FILES OFF)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_SET_DESTDIR ON)
set(CPACK_PACKAGING_INSTALL_PREFIX "/")
set(CPACK_PACKAGE_RELOCATABLE FALSE)

set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Matt Johnson")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${LINUXTRACK_PACKAGE_DESCRIPTION}")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS_PRIVATE_DIRS
    "${CMAKE_BINARY_DIR}/src"
    "${CMAKE_BINARY_DIR}/src/qt_gui"
    "${CMAKE_BINARY_DIR}/src/mickey"
    "${CMAKE_BINARY_DIR}/src/ltr_hotkey_gui"
    "${CMAKE_BINARY_DIR}/src/npclient_linux"
    "${CMAKE_BINARY_DIR}/src/scs_plugin")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    "${CMAKE_SOURCE_DIR}/packaging/cpack/postinst.sh;${CMAKE_SOURCE_DIR}/packaging/cpack/postrm.sh")

set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Amusements/Games")
set(CPACK_RPM_PACKAGE_URL "https://github.com/StarTuz/linuxtrack")
set(CPACK_RPM_PACKAGE_DESCRIPTION "${LINUXTRACK_PACKAGE_DESCRIPTION}")
set(CPACK_RPM_PACKAGE_AUTOREQPROV ON)
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE
    "${CMAKE_SOURCE_DIR}/packaging/cpack/postinst.sh")
set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE
    "${CMAKE_SOURCE_DIR}/packaging/cpack/postrm.sh")
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION
    "/opt"
    "/opt/linuxtrack"
    "/usr/bin"
    "/usr/lib"
    "/usr/lib/udev"
    "/usr/lib/udev/rules.d"
    "/usr/share"
    "/usr/share/applications"
    "/usr/share/icons"
    "/usr/share/icons/hicolor"
    "/usr/share/icons/hicolor/48x48"
    "/usr/share/icons/hicolor/48x48/apps"
    "/usr/share/icons/hicolor/scalable"
    "/usr/share/icons/hicolor/scalable/apps"
    "/usr/share/metainfo"
    "/usr/share/pixmaps")

include(CPack)
