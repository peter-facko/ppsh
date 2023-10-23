#pragma once
#include <string>

namespace ppsh
{
/** Represents a type of file redirection */
enum class redirection_type
{
    INPUT,
    OUTPUT_TRUNCATE,
    OUTPUT_APPEND,
};

/** Represents a file redirection in a command.
 *
 * Example: command <redir_input >redir_output_truncate >>redir_output_append
 * 		there are three different redirections in the example command
 */
struct redirection
{
    std::string file_path;
    redirection_type type;

    /** Constructs a valid redirection of 'file_path' with 'type'. */
    redirection(std::string file_path, redirection_type type);

    friend std::ostream& operator<<(std::ostream& out,
                                    const redirection& redirection);
};
}
