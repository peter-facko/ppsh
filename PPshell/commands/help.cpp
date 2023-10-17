#include <commands.hpp>

#include <iostream>

int PPshell::commands::help(std::ostream& output,
                            const po::options_description& description)
{
    output << description << "\n";

    output << "`PPshell` starts interactive mode.\n";
    output << "`PPshell <file_path>` executes the file as a script.\n";

    return 0;
}
