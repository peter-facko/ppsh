#include "process.hpp"
#include <commands.hpp>

#include <iostream>
#include <sstream>
#include <string_view>

int ppsh::commands::command(const std::string& command_string)
{
    std::istringstream command_stream(command_string);
    return process(command_stream);
}
