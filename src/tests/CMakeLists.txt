# src/tests/CMakeLists.txt
# Regression and unit test targets for linuxtrack.

# -------------------------------------------------------------------------
# test_lal — existing Catch2 tests for the lal module (no Qt dependency)
# -------------------------------------------------------------------------
# test_lal is built by src/lal/CMakeLists.txt; register it with CTest here.
add_test(NAME test_lal COMMAND test_lal)

# -------------------------------------------------------------------------
# test_webcam_info — regression tests for webcam_info.cpp static methods
# -------------------------------------------------------------------------
# Requires Qt6::Core for QString / QStringList used by webcam_info.
find_package(Qt6 COMPONENTS Core REQUIRED)

set(WEBCAM_INFO_TEST_SOURCES
    test_webcam_info.cpp
    stubs_dyn_load.cpp
    ../qt_gui/webcam_info.cpp
    catch2/catch_amalgamated.cpp
)

add_executable(test_webcam_info ${WEBCAM_INFO_TEST_SOURCES})

target_include_directories(test_webcam_info PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}          # for catch2/
    ${CMAKE_CURRENT_SOURCE_DIR}/..       # for dyn_load.h, list.h, webcam_driver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../qt_gui  # for webcam_info.h
)

target_link_libraries(test_webcam_info PRIVATE Qt6::Core)

# webcam_info.cpp uses QRegularExpression (Qt6) / QRegExp (Qt5)
target_compile_definitions(test_webcam_info PRIVATE)

add_test(NAME test_webcam_info COMMAND test_webcam_info)

# -------------------------------------------------------------------------
# test_image_process — guards shared blob extraction semantics
# -------------------------------------------------------------------------
set(IMAGE_PROCESS_TEST_SOURCES
    test_image_process.cpp
    stubs_utils.cpp
    ../image_process.c
    ../list.c
    catch2/catch_amalgamated.cpp
)

add_executable(test_image_process ${IMAGE_PROCESS_TEST_SOURCES})

target_include_directories(test_image_process PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_test(NAME test_image_process COMMAND test_image_process)

# -------------------------------------------------------------------------
# test_wc_driver_prefs — webcam face-tracker pref defaults and clamping
# -------------------------------------------------------------------------
set(WC_DRIVER_PREFS_TEST_SOURCES
    test_wc_driver_prefs.cpp
    ../wc_driver_prefs.c
    catch2/catch_amalgamated.cpp
)

add_executable(test_wc_driver_prefs ${WC_DRIVER_PREFS_TEST_SOURCES})

target_include_directories(test_wc_driver_prefs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_test(NAME test_wc_driver_prefs COMMAND test_wc_driver_prefs)
