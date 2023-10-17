#include <redirection.hpp>

#include <cassert>
#include <cstddef>
#include <iostream>

namespace PPshell
{
redirection::redirection(std::string file_path, redirection_type type)
    : file_path(std::move(file_path))
    , type(type)
{}

std::ostream& operator<<(std::ostream& out, const redirection& redirection)
{
    return out << "file_path: " << redirection.file_path
               << ", type: " << static_cast<int>(redirection.type);
}
}
