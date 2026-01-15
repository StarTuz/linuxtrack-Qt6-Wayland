# FindONNXRuntime.cmake
# =====================
#
# Find an ONNX Runtime installation.
# ONNX Runtime is a cross-platform inference and training machine-learning
# accelerator. This module is ported from OpenTrack.
#
# Input variables
# ---------------
# 
#   ONNXRuntime_ROOT            Set root installation (environment variable)
#   ONNXRuntime_DIR             CMake variable to set the installation root.
#
# Output variables
# ----------------
# 
#   ONNXRuntime_FOUND           True if headers and libraries were found
#   ONNXRuntime_LIBRARIES       Component libraries to be linked.
#   ONNXRuntime_INCLUDE_DIRS    Include directories.
#
# Imported Targets
# ----------------
#
#   onnxruntime::onnxruntime    Import target for linking

# Cache variable for the installation root
set(ONNXRuntime_DIR CACHE PATH "Root directory of the ONNX Runtime installation")

# Determine architecture string for nuget package subdir
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_SIZEOF_VOID_P GREATER_EQUAL 8)
        set(ONNXRuntime_Arch "linux-x64")
    else()
        message(STATUS "32 Bit Linux builds for ONNXRuntime not supported")
        set(ONNXRuntime_Arch "linux-x86-unsupported")
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(CMAKE_SIZEOF_VOID_P GREATER_EQUAL 8)
        set(ONNXRuntime_Arch "win-x64")
    else()
        set(ONNXRuntime_Arch "win-x86")
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(ONNXRuntime_Arch "osx-x64")
endif()

# Find the library
find_library(ONNXRuntime_LIBRARY onnxruntime
    HINTS 
        ${ONNXRuntime_DIR}
        $ENV{ONNXRuntime_ROOT}
        /usr
        /usr/local
    PATH_SUFFIXES 
        # For a "normal" installation
        "lib" "lib64" "bin"
        # For the nuget package
        "runtimes/${ONNXRuntime_Arch}/native"
)

# On Windows, also find the DLL for installation
if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES_BACKUP ${CMAKE_FIND_LIBRARY_SUFFIXES})
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll" ".DLL")
    find_library(ONNXRuntime_RUNTIME onnxruntime
        HINTS 
            ${ONNXRuntime_DIR}
            $ENV{ONNXRuntime_ROOT}
        PATH_SUFFIXES 
            "lib" "lib64" "bin"
            "runtimes/${ONNXRuntime_Arch}/native"
    )
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_BACKUP})
else()
    set(ONNXRuntime_RUNTIME ${ONNXRuntime_LIBRARY})
endif()

# Find the header
find_path(ONNXRuntime_INCLUDE_DIR onnxruntime_cxx_api.h
    HINTS 
        ${ONNXRuntime_DIR}
        $ENV{ONNXRuntime_ROOT}
        /usr
        /usr/local
    PATH_SUFFIXES
        # For nuget packages
        "build/native/include"
        # For when the onnx source repo structure is preserved
        "include/onnxruntime/core/session" 
        "include/onnxruntime"
        # For standard installations
        "include"
        "include/onnxruntime"
)

include(FindPackageHandleStandardArgs)
# Handle QUIETLY and REQUIRED arguments
find_package_handle_standard_args(ONNXRuntime DEFAULT_MSG
    ONNXRuntime_LIBRARY 
    ONNXRuntime_INCLUDE_DIR 
    ONNXRuntime_RUNTIME
)

if(ONNXRuntime_FOUND)
    if(NOT TARGET onnxruntime::onnxruntime)
        # Create an imported target for modern CMake
        add_library(onnxruntime::onnxruntime SHARED IMPORTED)
        set_target_properties(onnxruntime::onnxruntime PROPERTIES
            IMPORTED_LOCATION "${ONNXRuntime_RUNTIME}"
            INTERFACE_INCLUDE_DIRECTORIES "${ONNXRuntime_INCLUDE_DIR}"
            IMPORTED_IMPLIB "${ONNXRuntime_LIBRARY}"
        )
    endif()
    
    message(STATUS "ONNXRuntime found:")
    message(STATUS "  Library: ${ONNXRuntime_LIBRARY}")
    message(STATUS "  Include: ${ONNXRuntime_INCLUDE_DIR}")
endif()

mark_as_advanced(ONNXRuntime_INCLUDE_DIR ONNXRuntime_LIBRARY ONNXRuntime_RUNTIME)

set(ONNXRuntime_INCLUDE_DIRS ${ONNXRuntime_INCLUDE_DIR})
set(ONNXRuntime_LIBRARIES ${ONNXRuntime_LIBRARY})
