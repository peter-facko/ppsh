#include <pipes.hpp>

#include <charconv>
#include <cstdlib>
#include <iostream>

namespace ppsh::builtins
{
int exit(int argc, char** argv, int, int, int last_return_value)
{
    if (argc > 2)
    {
        std::cerr << "ppsh: exit: Too many arguments.\n";
        return 1;
    }

    const auto return_value =
        argc == 1 ? last_return_value : std::atoi(argv[1]);

    std::exit(return_value);
}
}
