#pragma once
#include <exception>
#include <string>

namespace PPshell::exceptions
{
class user_exception : public std::exception
{};

class wrapper_user_exception : public user_exception
{
private:
    const std::exception& inner_exception;

public:
    wrapper_user_exception(const std::exception& inner_exception) noexcept;

    const char* what() const noexcept override;
};

class message_user_exception : public user_exception
{
private:
    const std::string what_string;

public:
    message_user_exception(const std::string& what_string) noexcept;

    const char* what() const noexcept override;
};
}
