#include "tools.h"

void create_dir(const fl::path_t& dir_path) {
    std::filesystem::create_directory(dir_path);
}

void create_file(const fl::path_t& file_path) {
    std::filesystem::create_directory(file_path);

    FILE* cmake_file = fopen(file_path.c_str(), "w");
    fclose(cmake_file);
}

void create_file(const fl::path_t& file_path, const fl::vec_t<const fl::str_t> file_content) {
    std::filesystem::create_directory(file_path);

    FILE* cmake_file = fopen(file_path.c_str(), "w");
    for (auto& str : file_content) {
        fprintf(cmake_file, "%s\n", str.c_str());
    }
    fclose(cmake_file);
}