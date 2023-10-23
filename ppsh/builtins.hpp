#pragma once
#include <pipes.hpp>

#include <span>
#include <string_view>

namespace ppsh::builtins
{
typedef int(
    command_handler)(int, char**, int in, int out, int last_return_value);

/** Represents a command which is internal to the shell. */
struct info
{
    std::string_view name;
    command_handler* handler;
};

/** Gets the view of builtins. */
std::span<info> view();
}
