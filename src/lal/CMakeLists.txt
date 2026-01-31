add_library(lal STATIC
    lal_manager.h
    lal_manager.cpp
)

target_include_directories(lal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link against standard libraries (nlohmann_json is header only)
# If nlohmann_json needs to be found:
find_path(NLOHMANN_JSON_INCLUDE_DIR nlohmann/json.hpp)
if(NLOHMANN_JSON_INCLUDE_DIR)
    target_include_directories(lal PRIVATE ${NLOHMANN_JSON_INCLUDE_DIR})
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
