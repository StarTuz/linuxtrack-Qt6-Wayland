# src/wine_bridge/CMakeLists.txt

# Common flags for winegcc
set(WINE_CPPFLAGS -DHAVE_CONFIG_H -I${CMAKE_SOURCE_DIR} -I${CMAKE_BINARY_DIR} -I${CMAKE_SOURCE_DIR}/src)
set(WINE_COMMON_LDFLAGS -Wl,--no-warn-search-mismatch)

# Ensure bitness is handled
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(HAS_WINE64 ON)
else()
    set(HAS_WINE64 OFF)
endif()

option(ENABLE_WINE32 "Build 32-bit Wine bridge" ON)


# Helper to build a wine dll/exe
function(add_wine_binary TARGET_NAME)
    set(options SHARED)
    set(oneValueArgs OUTPUT BITNESS SPEC TYPE)
    set(multiValueArgs SOURCES LIBS DEFINES)
    cmake_parse_arguments(WINE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(WINE_ARCH_FLAG "")
    if(WINE_BITNESS EQUAL 32)
        set(WINE_ARCH_FLAG "-m32")
    endif()

    set(WINE_COMPILER ${WINEGCC})
    if(WINE_TYPE STREQUAL "CXX")
        set(WINE_COMPILER ${WINEGXX})
    endif()

    set(SPEC_FILE_ABS "")
    if(WINE_SPEC)
        set(SPEC_FILE_ABS "${CMAKE_CURRENT_SOURCE_DIR}/${WINE_SPEC}")
    endif()

    set(OBJ_FILES "")
    foreach(SRC ${WINE_SOURCES})
        get_filename_component(SRC_EXT ${SRC} EXT)
        get_filename_component(SRC_NAME ${SRC} NAME_WLE)
        # Avoid collisions for same source name in different bitness or types
        set(OBJ_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_${SRC_NAME}_${WINE_BITNESS}.o")
        
        # Determine source path
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")
            set(SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")
        elseif(EXISTS "${CMAKE_SOURCE_DIR}/src/${SRC}")
            set(SRC_PATH "${CMAKE_SOURCE_DIR}/src/${SRC}")
        else()
             message(FATAL_ERROR "Source ${SRC} not found for ${TARGET_NAME}")
        endif()

        if(SRC_EXT STREQUAL ".rc")
            add_custom_command(
                OUTPUT ${OBJ_FILE}
                COMMAND ${WRC} "-o" ${OBJ_FILE} "-I" "${CMAKE_CURRENT_SOURCE_DIR}" ${SRC_PATH}
                DEPENDS ${SRC_PATH}
                COMMENT "Compiling Wine resource ${SRC} for ${TARGET_NAME}"
            )
        else()
            set(COMP ${WINEGCC})
            if(SRC_EXT MATCHES "\\.(cpp|cxx|C)$")
                set(COMP ${WINEGXX})
            endif()
            
            set(EXTRA_DEF "")
            if(WINE_BITNESS EQUAL 64)
                set(EXTRA_DEF "-DFOR_WIN64=1")
            endif()

            add_custom_command(
                OUTPUT ${OBJ_FILE}
                COMMAND ${COMP} -c ${WINE_ARCH_FLAG} ${WINE_CPPFLAGS} ${EXTRA_DEF} ${WINE_DEFINES} -o ${OBJ_FILE} ${SRC_PATH}
                DEPENDS ${SRC_PATH} ${CMAKE_BINARY_DIR}/config.h
                COMMENT "Compiling Wine object ${SRC} (${WINE_BITNESS}-bit) for ${TARGET_NAME}"
            )
        endif()
        list(APPEND OBJ_FILES ${OBJ_FILE})
    endforeach()

    set(SHARED_FLAG "")
    if(WINE_SHARED)
        set(SHARED_FLAG "-shared")
    endif()

    set(RPATH_FLAGS "-Wl,-rpath,${CMAKE_INSTALL_PREFIX}/lib/linuxtrack:-Wl,-rpath,${CMAKE_INSTALL_PREFIX}/lib")

    add_custom_command(
        OUTPUT ${WINE_OUTPUT}
        COMMAND ${WINE_COMPILER} ${WINE_ARCH_FLAG} ${SHARED_FLAG} -o ${WINE_OUTPUT} ${OBJ_FILES} ${SPEC_FILE_ABS} ${WINE_COMMON_LDFLAGS} ${RPATH_FLAGS} ${WINE_LIBS}
        DEPENDS ${OBJ_FILES} ${SPEC_FILE_ABS}
        COMMENT "Linking Wine binary ${WINE_OUTPUT}"
    )
    add_custom_target(${TARGET_NAME} ALL DEPENDS ${WINE_OUTPUT})
    
    # Installation
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${WINE_OUTPUT} DESTINATION lib/linuxtrack)
endfunction()

# 1. NPClient
# 1. NPClient
if(ENABLE_WINE32)
add_wine_binary(NPClient32
    OUTPUT NPClient.dll.so
    BITNESS 32
    SHARED
    SPEC client/NPClient.spec
    SOURCES client/NPClient_main.c linuxtrack.c client/rest.c
    LIBS -ldl
)
endif()

if(HAS_WINE64)
    add_wine_binary(NPClient64
        OUTPUT NPClient64.dll.so
        BITNESS 64
        SHARED
        SPEC client/NPClient.spec
        SOURCES client/NPClient_main.c linuxtrack.c client/rest.c
        LIBS -ldl
    )
endif()

# 1b. NPClient UDP (Robust version - no Linux library dependencies)
# Uses Winsock to receive pose data from ltr_udp
# Uses simplified NPClient_udp_main.c that has no file/registry dependencies
if(ENABLE_WINE32)
add_wine_binary(NPClientUDP32
    OUTPUT NPClientUDP.dll.so
    BITNESS 32
    SHARED
    SPEC client/NPClient.spec
    SOURCES client/NPClient_udp_main.c client/linuxtrack_udp.c client/rest.c
    LIBS -lws2_32
    DEFINES -DUDP_BRIDGE
)
endif()

if(HAS_WINE64)
    add_wine_binary(NPClientUDP64
        OUTPUT NPClient64UDP.dll.so
        BITNESS 64
        SHARED
        SPEC client/NPClient.spec
        SOURCES client/NPClient_udp_main.c client/linuxtrack_udp.c client/rest.c
        LIBS -lws2_32
        DEFINES -DUDP_BRIDGE
    )
endif()

# 1c. Hotkey Utility (Native Win32 EXE for Proton/Wine)
# Built with MinGW to produce a proper PE executable (not winegcc .so format)
# This is required for reliable operation in Proton environments
find_program(MINGW32_GCC i686-w64-mingw32-gcc)
if(MINGW32_GCC)
    set(HOTKEY_EXE "${CMAKE_CURRENT_BINARY_DIR}/ltr_wine_hotkeys.exe")
    add_custom_command(
        OUTPUT ${HOTKEY_EXE}
        COMMAND ${MINGW32_GCC} 
            -o ${HOTKEY_EXE}
            -DHAVE_CONFIG_H
            -I${CMAKE_SOURCE_DIR}
            -I${CMAKE_BINARY_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/client/ltr_wine_hotkeys.c
            -lws2_32 -luser32 -lgdi32 -mwindows
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/client/ltr_wine_hotkeys.c
        COMMENT "Building ltr_wine_hotkeys.exe with MinGW (PE32)"
    )
    add_custom_target(ltr_wine_hotkeys ALL DEPENDS ${HOTKEY_EXE})
    install(FILES ${HOTKEY_EXE} DESTINATION lib/linuxtrack)
else()
    message(WARNING "MinGW (i686-w64-mingw32-gcc) not found - skipping ltr_wine_hotkeys.exe")
endif()

# 2. FreeTrackClient
# 2. FreeTrackClient
if(ENABLE_WINE32)
add_wine_binary(FreeTrackClient32
    OUTPUT FreeTrackClient.dll.so
    BITNESS 32
    SHARED
    SPEC ft_client/FreeTrackClient.spec
    SOURCES ft_client/FreeTrackClient_main.c linuxtrack.c
    LIBS -ldl
)
endif()

# 3. Controller
# 3. Controller
if(ENABLE_WINE32)
add_wine_binary(Controller
    TYPE CXX
    OUTPUT Controller.exe.so
    BITNESS 32
    SOURCES controller/main.cpp controller/resources.rc controller/dinput.cpp linuxtrack.c
    LIBS -lshell32 -ldxguid -ldinput -ldinput8 -luuid -ldl
)
endif()

# 4. Tester
# 4. Tester
if(ENABLE_WINE32)
add_wine_binary(Tester32
    TYPE CXX
    OUTPUT Tester.exe.so
    BITNESS 32
    SOURCES tester/main.cpp tester/npview.rc tester/rest.c tester/npifc.c
    LIBS -ldl
)
endif()

if(HAS_WINE64)
    add_wine_binary(Tester64
        TYPE CXX
        OUTPUT Tester64.exe.so
        BITNESS 64
        SOURCES tester/main.cpp tester/npview.rc tester/rest.c tester/npifc.c
        LIBS -ldl
    )
endif()

# 5. TrackIR (views stub)
# 5. TrackIR (views stub)
if(ENABLE_WINE32)
add_wine_binary(TrackIR_stub
    TYPE CXX
    OUTPUT TrackIR.exe.so
    BITNESS 32
    SOURCES views/main.cpp views/rest.c
)
endif()

# 6. FTC (FreeTrack Tester)
# 6. FTC (FreeTrack Tester)
if(ENABLE_WINE32)
add_wine_binary(FTC
    TYPE CXX
    OUTPUT ftc.exe.so
    BITNESS 32
    SOURCES ft_tester/main.cpp ft_tester/fttester.rc
)
endif()

# 7. CheckData
# 7. CheckData
if(ENABLE_WINE32)
add_wine_binary(CheckData
    OUTPUT check_data.exe.so
    BITNESS 32
    SOURCES client/check_data.c client/rest.c
)
endif()

# Documentation and icons
install(FILES README.wine DESTINATION share/doc/linuxtrack)
install(FILES linuxtrack.ico linuxtrack1.ico DESTINATION share/doc/linuxtrack)
