#pragma once

#include <flake/std/types/string.h>
#include <flake/std/types/path.h>
#include <flake/utils/command.h>

struct context_data_t {
    fl::path_t path;
    fl::command_t cmd;
};

context_data_t get_context(int argc, char** argv);