#pragma once
#include <pipes.hpp>
#include <redirections.hpp>

#include <cstdio>
#include <ostream>
#include <vector>

namespace PPshell
{
/** Represents a shell command
 *
 * Consists of:
 *      path,
 *      arguments,
 *      redirections
 *
 * Example format:
 * <filei1 >fileo1 command_path arg1 >fileo2 arg2 <filei2
 */
class command
{
    std::string command_path;
    std::vector<std::string> arguments;
    class redirections redirections;

    const std::optional<std::string>& get_input_file() const;
    const std::optional<std::string>& get_output_file() const;
    bool get_output_append() const;

public:
    /** Constructs a command with 'command_path', no arguments and no
     * redirections on success. */
    explicit command(std::string command_path);

    /** Appends an 'argument' to 'command'. */
    void register_argument(std::string argument);
    /** Registers 'redirections' in 'command'. */
    void register_redirections(class redirections redirections);

    /** Gets te 'command's path */
    const char* get_path() const;
    /** Gets the 'command's arguments as a raw null-terminated array of cstrings
     * with the first argument being the 'command's path. */
    std::vector<char*> get_argv();

    std::vector<const char*> get_argv() const;

    [[noreturn]] void execute_child(class pipes& pipes);

    /** Executes the command with pipe redirections 'pipes'. Returns child PID
     * (0 if parent) and the return value if available.
     *
     * Semantically: "pipes->input_pipe | command | pipes->output_pipe".
     */
    std::pair<pid_t, std::optional<int>> execute(class pipes& pipes,
                                                 int last_return_value);

    /** Prints debug info about the 'command' into 'out'. */
    friend std::ostream& operator<<(std::ostream& out, const command& command);

    void redirect_io();

    std::optional<int> try_execute_internal(class pipes& pipes,
                                            int last_return_value);
};
}
