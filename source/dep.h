#pragma once

#include <flake/std/types/vector.h>

#include "context.h"

namespace dep {
    void update_deps(const context_data_t& ctx, const fl::vec_t<fl::str_t>& deps);
    void exec_cmd(const context_data_t& ctx);
}