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
target_compile_features(lal PUBLIC cxx_std_17)
