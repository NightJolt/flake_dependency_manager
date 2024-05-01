#include <flake/std/types/string.h>
#include <flake/std/types/unordered_set.h>
#include <flake/std/types/vector.h>
#include <flake/std/types/array.h>

#include <cstdio>
#include <cassert>
#include <filesystem>

constexpr fl::arr_t<const char*, 3> submodules = {
    "core",
    "ecs",
    "rpc",
};

constexpr uint32_t submodule_index(fl::str_t submodule) {
    for (uint32_t i = 0; i < submodules.size(); i++) {
        if (submodules[i] == submodule) {
            return i;
        }
    }

    return -1;
}

static const fl::vec_t<fl::vec_t<uint32_t>> dependencies = {
    { },
    { submodule_index("core") },
    { submodule_index("core") },
};

struct dependency_data_t {
    const std::filesystem::path& path;
    fl::uset_t<uint32_t> resolved_dependencies;
};

void fetch_submodule(dependency_data_t& data, uint32_t index) {
    system((fl::str_t("cd ") + data.path.c_str() + " && git clone https://github.com/NightJolt/flake_" + submodules[index] + ".git").c_str());
}

void resolve_submodule(dependency_data_t& data, uint32_t submodule) {
    if (data.resolved_dependencies.contains(submodule)) {
        return;
    }

    fetch_submodule(data, submodule);

    data.resolved_dependencies.insert(submodule);

    for (uint32_t dependency : dependencies[submodule]) {
        resolve_submodule(data, dependency);
    }
}

void add_cmake(dependency_data_t& data) {
    std::filesystem::path empty_cpp_path = data.path;
    empty_cpp_path.append("empty.cpp");

    FILE* empty_cpp_file = fopen(empty_cpp_path.c_str(), "w");
    fclose(empty_cpp_file);

    std::filesystem::path cmake_path = data.path;
    cmake_path.append("CMakeLists.txt");

    FILE* cmake_file = fopen(cmake_path.c_str(), "w");
    fprintf(cmake_file, "cmake_minimum_required(VERSION 3.28)\n");
    fprintf(cmake_file, "project(flake_lib)\n");

    for (uint32_t submodule : data.resolved_dependencies) {
        fprintf(cmake_file, "add_subdirectory(flake_%s)\n", submodules[submodule]);
    }

    fprintf(cmake_file, "add_library(${PROJECT_NAME} STATIC empty.cpp)\n");

    for (uint32_t submodule : data.resolved_dependencies) {
        fprintf(cmake_file, "target_link_libraries(${PROJECT_NAME} PUBLIC flake_%s)\n", submodules[submodule]);
    }

    fclose(cmake_file);
}

int main(int argc, char** argv) {
    assert(argc >= 2);

    std::filesystem::path path = argv[1];
    path.append("flake");
    std::filesystem::create_directory(path);

    dependency_data_t data = { path, { } };

    for (int i = 2; i < argc; i++) {
        uint32_t index = submodule_index(argv[i]);

        resolve_submodule(data, index);
    }

    add_cmake(data);

    return 0;
}