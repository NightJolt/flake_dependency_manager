#include <filesystem>
#include <ranges>

#include "context.h"

context_data_t get_context(int argc, char** argv) {
    if (argc < 2) {
        throw std::invalid_argument("Not enough arguments");
    }

    int32_t cmd_size { argc - 1 };
    for (int i : std::views::iota(1, argc)) {
        cmd_size += strlen(argv[i]);
    }

    fl::str_t cmd_str;
    cmd_str.reserve(cmd_size);

    for (int i : std::views::iota(1, argc)) {
        cmd_str.append(argv[i]).append(" ");
    }
    
    context_data_t data;
    data.path = std::filesystem::current_path();
    data.cmd = fl::command_t(cmd_str);

    return data;
}