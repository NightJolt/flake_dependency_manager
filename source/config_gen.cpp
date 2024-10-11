#include "config_gen.h"
#include "tools.h"

bool contains_dep(const fl::vec_t<fl::str_t>& deps, const fl::str_t& dep) {
    return std::find(
        deps.begin(),
        deps.end(),
        dep
    ) != deps.end();
}

fl::vec_t<fl::str_t> get_deps(const context_data_t& ctx) {
    if (std::filesystem::exists(ctx.path))

    return {};
}

void add_dep(const fl::str_t& dep) {
    auto deps = get_deps();

    // if (deps.con)
}