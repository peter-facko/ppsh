#pragma once
#include <file_descriptor.hpp>

#include <cstddef>

namespace ppsh
{
/** Helper structure for creating pipes for pipelines. */
class pipes
{
    file_descriptor in;
    file_descriptor out;
    file_descriptor in_future;

public:
    /** Constructs invalid pipes. */
    pipes() noexcept;

private:
    void shift();

public:
    /** Shifts 'pipes' by one and opens a new pipe. */
    void create_next();

    /** Similar to `pipes_create_next`, but doesn't open a new pipe as this is
     * intented to be used before executing the last command in the pipeline. */
    void create_last();

    /** Redirects 'pipes's file descriptors to standard I/O and destroys
     * 'pipes'. */
    void redirect_and_destroy();

    /** Gets the raw input file descriptor of 'pipes'. */
    int get_raw_in() const;

    /** Gets the raw output file descriptor of 'pipes'. */
    int get_raw_out() const;

    friend std::ostream& operator<<(std::ostream& out, const pipes& pipes);
};
}
