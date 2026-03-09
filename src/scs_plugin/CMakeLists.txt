# CMakeLists.txt for SCS Linuxtrack Plugin
#
# Builds libscs_linuxtrack.so for ETS2/ATS head tracking.
#
# Usage:
#   cmake -B build -S .
#   cmake --build build
#   cp build/libscs_linuxtrack.so ~/.local/share/Steam/steamapps/common/Euro Truck Simulator 2/bin/linux_x64/plugins/

cmake_minimum_required(VERSION 3.10)
project(scs_linuxtrack VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find Linuxtrack library
find_library(LTR_LIBRARY 
    NAMES linuxtrack
    PATHS 
        /opt/linuxtrack/lib
        /usr/local/lib
        ${CMAKE_SOURCE_DIR}/../../build/src
    PATH_SUFFIXES linuxtrack
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../..  # For linuxtrack.h
    ${CMAKE_BINARY_DIR}/../..  # For config.h
)

# Plugin shared library
add_library(scs_linuxtrack SHARED
    scs_linuxtrack.cpp
)

# Link with Linuxtrack
if(LTR_LIBRARY)
    target_link_libraries(scs_linuxtrack ${LTR_LIBRARY})
else()
    # Direct source inclusion fallback
    target_sources(scs_linuxtrack PRIVATE
        ${CMAKE_SOURCE_DIR}/../linuxtrack.c
        ${CMAKE_SOURCE_DIR}/../ipc_utils.c
        ${CMAKE_SOURCE_DIR}/../utils.c
    )
    target_link_libraries(scs_linuxtrack m dl pthread)
endif()

# Platform-specific settings
if(UNIX AND NOT APPLE)
    set_target_properties(scs_linuxtrack PROPERTIES
        PREFIX ""  # No 'lib' prefix for plugin naming
        SUFFIX ".so"
        OUTPUT_NAME "scs_linuxtrack"
    )
    
    # RPATH for finding liblinuxtrack at runtime
    set_target_properties(scs_linuxtrack PROPERTIES
        INSTALL_RPATH "$ORIGIN:$ORIGIN/../lib/linuxtrack:/opt/linuxtrack/lib/linuxtrack"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()

# Installation
install(TARGETS scs_linuxtrack
    LIBRARY DESTINATION lib/linuxtrack
)

# Also install to a user-friendly location message
message(STATUS "scs_linuxtrack will be installed to lib/linuxtrack/")
message(STATUS "Copy manually to ETS2 plugins folder:")
message(STATUS "  ~/.local/share/Steam/steamapps/common/Euro Truck Simulator 2/bin/linux_x64/plugins/")
