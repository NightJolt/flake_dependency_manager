#pragma once

#include <filesystem>
#include <flake/std/types/vector.h>
#include <flake/std/types/string.h>
#include <flake/std/types/path.h>

void create_dir(const fl::path_t& dir_path);
void create_file(const fl::path_t& file_path);
void create_file(const fl::path_t& file_path, const fl::vec_t<const fl::str_t> file_content);