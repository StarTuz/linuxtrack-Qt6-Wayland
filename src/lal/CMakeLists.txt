add_library(lal STATIC
    lal_manager.h
    lal_manager.cpp
)

target_include_directories(lal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# nlohmann-json is required for JSON parsing (header-only library)
find_package(nlohmann_json QUIET)
if(nlohmann_json_FOUND)
    target_link_libraries(lal PRIVATE nlohmann_json::nlohmann_json)
else()
    # Fallback: try to find header directly
    find_path(NLOHMANN_JSON_INCLUDE_DIR nlohmann/json.hpp
        PATHS /usr/include /usr/local/include)
    if(NLOHMANN_JSON_INCLUDE_DIR)
        target_include_directories(lal PRIVATE ${NLOHMANN_JSON_INCLUDE_DIR})
    else()
        message(FATAL_ERROR "nlohmann-json not found. Install with:\n"
            "  Arch/EndeavourOS: sudo pacman -S nlohmann-json\n"
            "  Debian/Ubuntu:    sudo apt install nlohmann-json3-dev\n"
            "  Fedora:           sudo dnf install nlohmann-json-devel")
    endif()
endif()

# Since we use std::filesystem (C++17), ensure standards
# Test Executable
add_executable(test_lal test_lal.cpp)
target_include_directories(test_lal PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "../tests/catch2")
target_link_libraries(test_lal PRIVATE lal)
# Link amalgamated Catch2 implementation
target_sources(test_lal PRIVATE ../tests/catch2/catch_amalgamated.cpp)


# Install the manifest
install(FILES lal_manifest.json DESTINATION ${CMAKE_INSTALL_DATADIR}/linuxtrack)
