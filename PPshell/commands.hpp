#pragma once
#include <program_options.hpp>

#include <filesystem>
#include <iosfwd>
#include <string_view>

namespace PPshell::commands
{
int help(std::ostream& output, const po::options_description& description);

int command(const std::string& command);

int file(const std::filesystem::path& file_path);

int interactive();
}
