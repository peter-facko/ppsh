#include <pipes.hpp>

#include <file_descriptor.hpp>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <unistd.h>

namespace ppsh
{
pipes::pipes() noexcept
    : in()
    , out()
    , in_future()
{}

void pipes::shift()
{
    out.close();
    in = std::move(in_future);
}

void pipes::create_next()
{
    shift();
    std::tie(in_future, out) = file_descriptor::construct_pipe();
}

void pipes::create_last()
{
    shift();
}

void pipes::redirect_and_destroy()
{
    in.redirect_and_destroy(STDIN_FILENO);
    out.redirect_and_destroy(STDOUT_FILENO);
    in_future.close();
}

int pipes::get_raw_in() const
{
    return in.get_raw_or(STDIN_FILENO);
}

int pipes::get_raw_out() const
{
    return out.get_raw_or(STDOUT_FILENO);
}

std::ostream& operator<<(std::ostream& out, const pipes& pipes)
{
    return out << "pipes:"
               << "\tin: " << pipes.in << "\n"
               << "\tout: " << pipes.out << "\n"
               << "\tin_future: " << pipes.in_future << "\n";
}
}
