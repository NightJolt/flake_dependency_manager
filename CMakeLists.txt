cmake_minimum_required(VERSION 3.28)

project(flake)

add_subdirectory(flake_core)

include_directories(source)

file(
    GLOB_RECURSE flake_sources
    
    source/*.cpp
)

add_executable(${PROJECT_NAME} ${flake_sources})
# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23 CXX_EXTENSIONS OFF)

target_link_libraries(${PROJECT_NAME} PUBLIC flake_core)