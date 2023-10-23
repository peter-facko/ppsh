#include <pipeline.hpp>

#include <command.hpp>
#include <exceptions.hpp>
#include <pipes.hpp>

#include <cassert>
#include <cstdio>
#include <cstring>
#include <format>
#include <optional>
#include <sys/wait.h>
#include <unistd.h>

namespace ppsh
{
static int child_status_to_return_value(int status)
{
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else
    {
        if (WIFSIGNALED(status))
        {
            int child_signal = WTERMSIG(status);
            fprintf(stderr, "Killed by signal %d\n", child_signal);
            return 128 + child_signal;
        }
        else
            return status;
    }
}

static int wait_for_last_child(pid_t id)
{
    assert(id >= 0);

    if (id == 0)
    {
        return 0;
    }

    int status;

    if (waitpid(id, &status, 0) == (pid_t)-1)
    {
        fprintf(stderr, "wait failed\n");
        throw 1;
    }

    return child_status_to_return_value(status);
}

static void wait_for_children(const std::size_t children_count)
{
    auto success_all = true;

    for (auto i = 0uz; i != children_count; ++i)
    {
        const auto success_this = wait(nullptr);

        if (success_this == -1)
        {
            success_all = false;
        }
    }

    if (!success_all)
    {
        throw exceptions::message_user_exception(
            std::format("Wait failed: {}\n", std::strerror(errno)));
    }
}

static void pipes_create(class pipes& pipes, const bool is_last_command)
{
    // std::cerr << "pipes_create: last_command = " << is_last_command << "\n";

    return !is_last_command ? pipes.create_next() : pipes.create_last();
}

std::tuple<pid_t, std::optional<int>, std::size_t> pipeline::spawn_children(
    int last_return_value)
{
    class pipes pipes;

    pid_t last_child_pid = -1;
    std::size_t spawned_children_count = 0;
    std::optional<int> last_child_return_value;

    for (auto cmd_i = commands.begin(); cmd_i != commands.end(); ++cmd_i)
    {
        pipes_create(pipes, cmd_i == commands.end() - 1);

        const auto [child_pid, return_value_opt] =
            cmd_i->execute(pipes, last_return_value);

        if (child_pid == -1)
        {
            throw 1;
        }

        last_child_pid = child_pid;
        last_child_return_value = return_value_opt;

        // was not a builtin
        if (!return_value_opt)
        {
            ++spawned_children_count;
        }
    }

    return {last_child_pid, last_child_return_value, spawned_children_count};
}

std::ostream& operator<<(std::ostream& out, const pipeline& pipeline)
{
    for (auto& command : pipeline.commands)
    {
        out << command << " |\n";
    }
    return out;
}

int pipeline::execute(int last_return_value)
{
    const auto [last_child_pid,
                last_child_return_value,
                spawned_children_count] = spawn_children(last_return_value);

    const auto return_value = last_child_return_value
                                  ? *last_child_return_value
                                  : wait_for_last_child(last_child_pid);

    if (spawned_children_count != 0)
    {
        wait_for_children(spawned_children_count - 1);
    }

    return return_value;
}
}
