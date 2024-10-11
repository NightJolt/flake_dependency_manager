#include <flake/std/types/string.h>
#include <flake/std/types/unordered_set.h>
#include <flake/std/types/vector.h>
#include <flake/std/types/array.h>
#include <flake/std/types/path.h>

#include <cstdio>

#include "dep.h"
#include "tools.h"

constexpr fl::arr_t<const char*, 5> submodules {
    "none",
    "core",
    "ecs",
    "net",
    "rpc",
};

constexpr uint32_t submodule_index(const fl::str_t& submodule) {
    for (uint32_t i = 0; i < submodules.size(); i++) {
        if (submodules[i] == submodule) {
            return i;
        }
    }

    return -1;
}

constexpr fl::arr_t<fl::arr_t<uint32_t, submodules.size()>, submodules.size()> dependencies {
    fl::arr_t<uint32_t, submodules.size()> { },
    { },
    { submodule_index("core") },
    { submodule_index("core") },
    { submodule_index("net") }
};

struct dependency_data_t {
    fl::uset_t<uint32_t> resolved_dependencies;
};

void fetch_submodule(const context_data_t& ctx, dependency_data_t& data, uint32_t index) {
    system((fl::str_t("cd ") + ctx.path.c_str() + " && git clone https://github.com/NightJolt/flake_" + submodules[index] + ".git").c_str());
}

void resolve_submodule(const context_data_t& ctx, dependency_data_t& data, uint32_t submodule) {
    if (data.resolved_dependencies.contains(submodule)) {
        return;
    }

    fetch_submodule(ctx, data, submodule);

    data.resolved_dependencies.insert(submodule);

    for (uint32_t dependency : dependencies[submodule]) {
        if (dependency == 0) {
            continue;
        }

        resolve_submodule(ctx, data, dependency);
    }
}

void add_cmake(const context_data_t& ctx, dependency_data_t& data) {
    create_file(ctx.path/"empty.cpp");

    fl::path_t cmake_path { ctx.path/"CMakeLists.txt" };
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

void dep::update_deps(const context_data_t& ctx, const fl::vec_t<fl::str_t>& deps) {
    std::filesystem::create_directory(ctx.path);

    dependency_data_t data;

    for (auto& dep : deps) {
        uint32_t index = submodule_index(dep);

        if (index == 0) {
            continue;
        }

        resolve_submodule(ctx, data, index);
    }

    add_cmake(ctx, data);
}

void dep::exec_cmd(const context_data_t& ctx) {
    const auto& args = ctx.cmd.get_args();
    const auto& argc = args.size();

    if (argc < 1) {
        return;
    }

    fl::vec_t<fl::str_t> deps;
    deps.reserve(argc);

    for (const auto& arg : args) {
        deps.emplace_back(arg);
    }

    update_deps(ctx, deps);
}