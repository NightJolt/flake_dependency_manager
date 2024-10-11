#include <flake/std/types/string.h>

#include "dep.h"
#include "init.h"
#include "context.h"

int main(int argc, char** argv) {
    try {
        auto ctx = get_context(argc, argv);
        const fl::str_t& cmd = ctx.cmd.get_command();

        if (cmd.compare("dep") == 0) {
            dep::exec_cmd(ctx);
        } else if (cmd.compare("init") == 0) {
            init::exec_cmd(ctx);
        }
    } catch(const std::exception& e) {
        printf("%s\n", e.what());
    }

    return 0;
}