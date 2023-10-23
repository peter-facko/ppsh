#include <commands.hpp>

#include <iostream>

int ppsh::commands::help(std::ostream& output,
                         const po::options_description& description)
{
    output << description << "\n";

    output << "`ppsh` starts interactive mode.\n";
    output << "`ppsh <file_path>` executes the file as a script.\n";

    return 0;
}
