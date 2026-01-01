# Install script for directory: /home/startux/Code/linuxtrackfixed/linuxtrack/src

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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack32.so.0" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack32.so.0")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack32.so.0"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/liblinuxtrack32.so.0")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack32.so.0" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack32.so.0")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack32.so.0"
         OLD_RPATH ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack32.so.0")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/liblinuxtrack32.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/linuxtrack32.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/lal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/ltr_hotkey_gui/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltr.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltr.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltr.so"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/libltr.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltr.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltr.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltr.so"
         OLD_RPATH ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltr.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/ltr.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack.so.0" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack.so.0")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack.so.0"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/liblinuxtrack.so.0")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack.so.0" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack.so.0")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack.so.0"
         OLD_RPATH "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/liblinuxtrack.so.0")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/liblinuxtrack.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/linuxtrack.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libwc.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libwc.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libwc.so"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE MODULE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/libwc.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libwc.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libwc.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libwc.so"
         OLD_RPATH "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libwc.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/wc.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libtir.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libtir.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libtir.so"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE MODULE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/libtir.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libtir.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libtir.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libtir.so"
         OLD_RPATH "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libtir.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/tir.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libjoy.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libjoy.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libjoy.so"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE MODULE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/libjoy.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libjoy.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libjoy.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libjoy.so"
         OLD_RPATH "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libjoy.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/joy.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libft.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libft.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libft.so"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE MODULE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/libft.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libft.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libft.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libft.so"
         OLD_RPATH "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libft.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/ft.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltusb1.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltusb1.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltusb1.so"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE MODULE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/libltusb1.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltusb1.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltusb1.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltusb1.so"
         OLD_RPATH "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/libltusb1.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/ltusb1.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9.so"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/xlinuxtrack9.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9.so"
         OLD_RPATH ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/xlinuxtrack9.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9_32.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9_32.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9_32.so"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack" TYPE SHARED_LIBRARY FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/xlinuxtrack9_32.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9_32.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9_32.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9_32.so"
         OLD_RPATH ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/xlinuxtrack9_32.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/xlinuxtrack9_32.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/ltr_server1")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1"
         OLD_RPATH "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_server1")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/ltr_server1.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/ltr_recenter")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter"
         OLD_RPATH "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_recenter")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/ltr_recenter.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_pipe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_pipe")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_pipe"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/ltr_pipe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_pipe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_pipe")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_pipe"
         OLD_RPATH ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_pipe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/ltr_pipe.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_extractor" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_extractor")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_extractor"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/ltr_extractor")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_extractor" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_extractor")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_extractor"
         OLD_RPATH ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_extractor")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/ltr_extractor.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_udp" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_udp")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_udp"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/ltr_udp")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_udp" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_udp")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_udp"
         OLD_RPATH ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_udp")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/ltr_udp.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/osc_server" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/osc_server")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/osc_server"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/osc_server")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/osc_server" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/osc_server")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/osc_server"
         OLD_RPATH ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/osc_server")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/osc_server.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_hotkeyd" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_hotkeyd")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_hotkeyd"
         RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/ltr_hotkeyd")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_hotkeyd" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_hotkeyd")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_hotkeyd"
         OLD_RPATH ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/opt/linuxtrack/lib/linuxtrack:/opt/linuxtrack/lib:\$ORIGIN/../lib/linuxtrack:\$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ltr_hotkeyd")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/CMakeFiles/ltr_hotkeyd.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/linuxtrack" TYPE FILE FILES
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/linuxtrack1.conf"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/linuxtrack.c"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/linuxtrack_hello_world.c"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/linuxtrack_hello_world_adv.c"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/linuxtrack" TYPE FILE FILES
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/99-TIR.rules"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/99-Mickey.rules"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/home/startux/Code/linuxtrackfixed/linuxtrack/src/linuxtrack.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/ltr_gui.1"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/ltr_server1.1"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/mickey.1"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/ltr_extractor.1"
    "/home/startux/Code/linuxtrackfixed/linuxtrack/src/ltr_pipe.1"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
