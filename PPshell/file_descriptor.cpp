#include <file_descriptor.hpp>

#include <exceptions.hpp>

#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <format>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <utility>

namespace PPshell
{
file_descriptor::file_descriptor() noexcept
    : fd()
{}
file_descriptor::file_descriptor(file_descriptor&& other) noexcept
    : fd(std::exchange(other.fd, {}))
{}

file_descriptor& file_descriptor::operator=(file_descriptor&& other) noexcept
{
    fd = std::exchange(other.fd, {});
    return *this;
}

file_descriptor::file_descriptor(int raw_fd) noexcept
    : fd(raw_fd)
{}

file_descriptor file_descriptor::open(const std::string& path,
                                      int flags,
                                      mode_t mode)
{
    const auto result = ::open(path.c_str(), flags, mode);

    if (result == -1)
    {
        throw exceptions::message_user_exception(
            std::format("File opening failed: {}.", strerror(errno)));
    }

    // std::cerr << "Opened " << path << " as " << result << "\n";

    return file_descriptor(result);
}

void file_descriptor::close()
{
    if (fd)
    {
        ::close(*fd);
    }

    fd.reset();
}

file_descriptor::~file_descriptor()
{
    close();
}

file_descriptor::operator bool() const
{
    return fd.has_value();
}

std::pair<file_descriptor, file_descriptor> file_descriptor::construct_pipe()
{
    int temp[2];

    const auto success = pipe(temp);
    if (success == -1)
    {
        throw std::runtime_error(
            std::format("Pipe creation failed: {}.", strerror(errno)));
    }

    return std::make_pair(file_descriptor(temp[0]), file_descriptor(temp[1]));
}

void file_descriptor::redirect_and_destroy(int where)
{
    assert(where >= 0);

    if (!fd)
    {
        return;
    }

    if (*fd == where)
    {
        throw std::runtime_error("Bad redirection.");
    }

    // std::cerr << "Redirecting " << *fd << " to " << where << "\n";

    const auto success = ::dup2(*fd, where);
    if (success == -1)
    {
        throw std::runtime_error(
            std::format("File descriptor duplication failed: {}.",
                        strerror(errno)));
    }

    close();
}

int file_descriptor::get_raw_or(int default_value) const noexcept
{
    return fd.value_or(default_value);
}

std::ostream& operator<<(std::ostream& out, const file_descriptor& file)
{
    return out << std::format("FD({})", file.fd.value_or(-1));
}
}
