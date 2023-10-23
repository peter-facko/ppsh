#include <builtins.hpp>

#include <string.h>

namespace ppsh::builtins
{
command_handler exit;
command_handler cd;

static info builtins[2] = {{"exit", exit}, {"cd", cd}};

std::span<info> view()
{
    return std::span(builtins);
}
}
