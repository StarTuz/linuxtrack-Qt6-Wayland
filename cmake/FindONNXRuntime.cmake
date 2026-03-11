include(FindPackageHandleStandardArgs)

set(ONNXRuntime_DIR CACHE PATH "Root directory of the ONNX Runtime installation")

if(DEFINED ENV{ONNXRuntime_ROOT} AND NOT ONNXRuntime_DIR)
    set(ONNXRuntime_DIR "$ENV{ONNXRuntime_ROOT}")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(ONNXRuntime_Arch "linux-x64")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ONNXRuntime_Arch "win-x64")
    else()
        set(ONNXRuntime_Arch "win-x86")
    endif()
endif()

find_library(ONNXRuntime_LIBRARY onnxruntime
    HINTS ${ONNXRuntime_DIR}
    PATH_SUFFIXES
        lib
        lib64
        lib/Release
        runtimes/${ONNXRuntime_Arch}/native
)

if(WIN32)
    find_library(ONNXRuntime_RUNTIME onnxruntime
        HINTS ${ONNXRuntime_DIR}
        PATH_SUFFIXES
            lib
            lib64
            bin
            runtimes/${ONNXRuntime_Arch}/native
    )
else()
    set(ONNXRuntime_RUNTIME ${ONNXRuntime_LIBRARY})
endif()

find_path(ONNXRuntime_INCLUDE_DIR onnxruntime_cxx_api.h
    HINTS ${ONNXRuntime_DIR}
    PATH_SUFFIXES
        include
        include/onnxruntime
        include/onnxruntime/core/session
)

find_package_handle_standard_args(ONNXRuntime DEFAULT_MSG
    ONNXRuntime_LIBRARY ONNXRuntime_INCLUDE_DIR ONNXRuntime_RUNTIME)

if(ONNXRuntime_FOUND AND NOT TARGET onnxruntime::onnxruntime)
    add_library(onnxruntime::onnxruntime SHARED IMPORTED)
    set_target_properties(onnxruntime::onnxruntime PROPERTIES
        IMPORTED_LOCATION "${ONNXRuntime_RUNTIME}"
        IMPORTED_IMPLIB "${ONNXRuntime_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${ONNXRuntime_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(ONNXRuntime_INCLUDE_DIR ONNXRuntime_LIBRARY ONNXRuntime_RUNTIME)

set(ONNXRuntime_INCLUDE_DIRS ${ONNXRuntime_INCLUDE_DIR})
set(ONNXRuntime_LIBRARIES ${ONNXRuntime_LIBRARY})
