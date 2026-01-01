# Install script for directory: /home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/opt/linuxtrack")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_gui" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_gui")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_gui"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/ltr_gui")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_gui" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_gui")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_gui"
         OLD_RPATH "/home/startux/Code/linuxtrackfixed/linuxtrack/src::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_gui")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/CMakeFiles/ltr_gui.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/linuxtrack" TYPE FILE FILES
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/sparow_opaq.obj"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/sparow_glass.obj"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/xm8_detail.png"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/sources.txt"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/spec.txt"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/sphere.obj"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/sphere.png"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/sources_mfc.txt"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/win7.reg"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/linuxtrack/help/ltr_gui" TYPE FILE FILES
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/help.qhc"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/help.qch"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/startux/Code/linuxtrackfixed/linuxtrack/src/qt_gui/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
