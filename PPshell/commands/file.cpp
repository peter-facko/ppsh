#include <commands.hpp>
#include <exceptions.hpp>
#include <process.hpp>

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string_view>

int PPshell::commands::file(const std::filesystem::path& file_path)
{
    std::ifstream file(file_path);

    if (file.fail())
    {
        throw exceptions::message_user_exception(
            std::format("Failed to open file `{}`.", file_path.string()));
    }

    return process(file);
}
