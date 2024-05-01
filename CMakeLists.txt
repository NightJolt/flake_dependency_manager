cmake_minimum_required(VERSION 3.28)

project(flake_dependency_manager)

add_subdirectory(flake_core)

include_directories(source)

file(
    GLOB_RECURSE flake_dm_source

    source/*.h
    source/*.cpp
)

add_executable(${PROJECT_NAME} ${flake_dm_source})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)

target_link_libraries(${PROJECT_NAME} PUBLIC flake_core)

