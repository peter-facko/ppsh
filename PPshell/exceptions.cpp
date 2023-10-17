#include <exceptions.hpp>

namespace PPshell::exceptions
{
wrapper_user_exception::wrapper_user_exception(
    const std::exception& inner_exception) noexcept
    : inner_exception(inner_exception)
{}

const char* wrapper_user_exception::what() const noexcept
{
    return inner_exception.what();
}

message_user_exception::message_user_exception(
    const std::string& what_string) noexcept
    : what_string(what_string)
{}

const char* message_user_exception::what() const noexcept
{
    return what_string.c_str();
}
}
