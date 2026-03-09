# Native Linux NPClient Library
# Provides TrackIR interface for native Linux games

cmake_minimum_required(VERSION 3.10)
project(npclient_linux VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/..  # For linuxtrack.h
    ${CMAKE_BINARY_DIR}/..  # For config.h if needed
)

# 64-bit library (primary) - named differently from Wine bridge targets
add_library(npclient_native SHARED npclient.c)
target_link_libraries(npclient_native PRIVATE linuxtrack)
set_target_properties(npclient_native PROPERTIES
    PREFIX "lib"
    OUTPUT_NAME "NPClient64"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# 32-bit library (for compatibility, if multi-lib available)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # Check if we can build 32-bit
    include(CheckCCompilerFlag)
    check_c_compiler_flag("-m32" COMPILER_SUPPORTS_M32)
    
    if(COMPILER_SUPPORTS_M32)
        add_library(NPClient SHARED npclient.c)
        target_compile_options(NPClient PRIVATE -m32)
        target_link_options(NPClient PRIVATE -m32)
        # Note: Requires 32-bit liblinuxtrack to be available
        target_link_libraries(NPClient PRIVATE linuxtrack32)
        set_target_properties(NPClient PROPERTIES
            PREFIX "lib"
            OUTPUT_NAME "NPClient"
            VERSION ${PROJECT_VERSION}
            SOVERSION 1
        )
    endif()
endif()

# Symbol visibility - export NP_* functions
set_target_properties(npclient_native PROPERTIES
    C_VISIBILITY_PRESET default
)

# RPATH for finding liblinuxtrack
set_target_properties(npclient_native PROPERTIES
    INSTALL_RPATH "$ORIGIN:$ORIGIN/../lib/linuxtrack:/opt/linuxtrack/lib/linuxtrack"
    BUILD_WITH_INSTALL_RPATH TRUE
)

# Installation
install(TARGETS npclient_native
    LIBRARY DESTINATION lib/linuxtrack
)

if(TARGET NPClient)
    install(TARGETS NPClient
        LIBRARY DESTINATION lib/linuxtrack
    )
endif()

message(STATUS "npclient_native will be built for native Linux TrackIR emulation")
