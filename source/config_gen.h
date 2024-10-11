#pragma once

#include <flake/std/types/vector.h>
#include <flake/std/types/string.h>

bool contains_dep(const fl::vec_t<fl::str_t>& deps, const fl::str_t& dep);
fl::vec_t<fl::str_t> get_deps();
void add_dep(const fl::str_t& dep);