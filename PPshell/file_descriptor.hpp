#pragma once
#include <fcntl.h>
#include <optional>
#include <ostream>
#include <string>
#include <tuple>

namespace PPshell
{
/** Represents an open file descriptor or an invalid state. */
class file_descriptor
{
    friend class pipes;

    std::optional<int> fd;

public:
    /** Constructs an invalid file descriptor. */
    file_descriptor() noexcept;

    file_descriptor(file_descriptor&&) noexcept;

    file_descriptor& operator=(file_descriptor&&) noexcept;

private:
    /** Constructs a file descriptor equivalent to 'raw_fd'. */
    explicit file_descriptor(int raw_fd) noexcept;

public:
    /** Constructs 'fd' using `open(path, flags, mode)`. */
    static file_descriptor open(const std::string& path,
                                int flags,
                                mode_t mode);

private:
    void close();

    int get_raw_or(int default_value) const noexcept;

public:
    ~file_descriptor();

    /** Constructs two file descriptors (in, out) with ends of a new unnamed
     * pipe. */
    static std::pair<file_descriptor, file_descriptor> construct_pipe();

    /** Redirects 'where' to 'this'. Invalidates 'this'. */
    void redirect_and_destroy(int where);

    /** Returns true iff 'this' is valid. */
    explicit operator bool() const;

    friend std::ostream& operator<<(std::ostream& out,
                                    const file_descriptor& fd);
};
}
