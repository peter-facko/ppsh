#pragma once
#include <redirection.hpp>

#include <optional>
#include <string>

namespace ppsh
{
/** Represents all file redirections of a command. */
class redirections
{
    friend class command;

    std::optional<std::string> input_file;
    std::optional<std::string> output_file;
    bool output_append;

public:
    /** Constructs a valid empty 'redirections'. */
    redirections();

    /** Overrides the 'redirections' in 'redirections'. */
    void register_redirection(redirection redirection);

    /** Combines 'other' into 'redirections', with 'other' having the higher
     * priority.
     */
    void combine(redirections other);

    /** Redirects standard I/O to 'this'. */
    void redirect_io();

private:
    int get_append_flag() const;
};
}
