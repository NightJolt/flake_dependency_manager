#include <cstdio>
#include <filesystem>

#include <flake/std/types/array.h>

#include "init.h"
#include "dep.h"
#include "json.h"
#include "tools.h"



struct init_data_t {
    fl::path_t super_path;
    fl::path_t proj_path;
    fl::str_t proj_name;
};









void add_source_folder(const init_data_t& data) {
    fl::path_t sources_path = data.proj_path/"source";
    create_dir(sources_path);

    create_file(sources_path/"main.cpp");
}

void add_cmake(const init_data_t& data) {
    create_file(data.proj_path/"CMakeLists.txt", fl::vec_t<const fl::str_t> {
        "cmake_minimum_required(VERSION 3.28)",
        fl::str_t("project(").append(data.proj_name).append(")"),
        "add_subdirectory(flake)"
        "file(GLOB_RECURSE sources source/*.cpp source/*.h)",
        "add_executable(${PROJECT_NAME} ${sources})",
        "target_include_directories(${PROJECT_NAME} PRIVATE source)",
        "target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)",
        "target_link_libraries(${PROJECT_NAME} PRIVATE flake_lib)",
    });
}

void add_gitignore(const init_data_t& data) {
    create_file(data.proj_path/".gitignore", fl::vec_t<const fl::str_t> {
        ".vscode",
        "build",
        "flake",
    });
}

void add_flake_config(const init_data_t& data) {
    nlohmann::json json;
    json["dep"] = { "core" };

    create_file(data.proj_path/"flake_config.txt", fl::vec_t<const fl::str_t> {
        json.dump(4).c_str(),
    });
}


void init_proj(const context_data_t& ctx, const init_data_t& data) {
    std::filesystem::create_directory(data.proj_path);

    add_source_folder(data);
    add_cmake(data);
    add_gitignore(data);
    add_flake_config(data);

    dep::update_deps(ctx, { "core" });
}

void init::exec_cmd(context_data_t& ctx) {
    init_data_t data;
    data.super_path = ctx.path;
    data.proj_name = ctx.cmd.get_args()[0];
    data.proj_path = data.super_path/data.proj_name;

    ctx.path.append(data.proj_name);

    init_proj(ctx, data);
}