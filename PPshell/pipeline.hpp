#pragma once
#include <iostream>
#include <utility>
#include <vector>

#include <command.hpp>

namespace PPshell
{
/** Represents a non-empty shell pipeline (sequence of piped commands).
 *
 * Example: command1 | command2 | command3
 */
class pipeline
{
    std::vector<command> commands;

    std::tuple<pid_t, std::optional<int>, std::size_t> spawn_children(
        int last_return_value);

public:
    /** Constructs a valid pipeline with 'commands'. */
    explicit pipeline(auto&& commands)
        : commands()
    {
        for (auto&& command : commands)
        {
            this->commands.emplace_back(
                std::forward<decltype(command)>(command));
        }
    }

    /** Executes the pipeline, returning its return value. 'last_return_value'
     * is the return value of the last executed pipeline. */
    int execute(int last_return_value);

    /** Prints debug info about 'pipeline' into 'out'. */
    friend std::ostream& operator<<(std::ostream& out,
                                    const pipeline& pipeline);
};
}
