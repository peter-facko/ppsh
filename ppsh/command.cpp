#include <command.hpp>

#include <builtins.hpp>
#include <exceptions.hpp>
#include <redirections.hpp>

#include <algorithm>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <format>
#include <functional>
#include <ios>
#include <iostream>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string_view>
#include <unistd.h>
#include <vector>

namespace ppsh
{
command::command(std::string command_path)
    : command_path(std::move(command_path))
    , arguments()
    , redirections()
{}

void command::register_argument(std::string argument)
{
    arguments.push_back(std::move(argument));
}

void command::register_redirections(class redirections redirections)
{
    this->redirections = std::move(redirections);
}

const char* command::get_path() const
{
    return command_path.c_str();
}

std::vector<char*> command::get_argv()
{
    std::vector<char*> argv;
    argv.push_back(command_path.data());

    for (auto& argument : arguments)
    {
        argv.push_back(argument.data());
    }

    argv.push_back(nullptr);

    return argv;
}

std::vector<const char*> command::get_argv() const
{
    auto argv = const_cast<command&>(*this).get_argv();

    return std::vector<const char*>(argv.begin(), argv.end());
}

void command::redirect_io()
{
    redirections.redirect_io();
}

static ppsh::builtins::info* find_builtin_with_name(std::string_view name)
{
    const auto commands_view = ppsh::builtins::view();

    const auto i = std::ranges::find_if(commands_view,
                                        [name](ppsh::builtins::info info)
                                        {
                                            return name == info.name;
                                        });

    return i != commands_view.end() ? i.base() : nullptr;
}

std::optional<int> command::try_execute_internal(class pipes& pipes,
                                                 int last_return_value)
{
    ppsh::builtins::info* internal_command_ptr =
        find_builtin_with_name(get_path());

    if (!internal_command_ptr)
    {
        return std::nullopt;
    }

    // std::cerr << "Executing builtin with in = " << pipes.get_raw_in()
    //   << ", out = " << pipes.get_raw_out() << "\n";

    return internal_command_ptr->handler(arguments.size() + 1,
                                         get_argv().data(),
                                         pipes.get_raw_in(),
                                         pipes.get_raw_out(),
                                         last_return_value);
}

void command::execute_child(class pipes& pipes)
{
    signal(SIGINT, SIG_DFL);

    // std::cerr << pipes << "\n";

    pipes.redirect_and_destroy();

    const auto path = get_path();

    redirect_io();

    execvp(path, get_argv().data());

    throw exceptions::message_user_exception(
        std::format("Couldn't execute '{}': {}.", path, std::strerror(errno)));
}

std::pair<pid_t, std::optional<int>> command::execute(class pipes& pipes,
                                                      int last_return_value)
{
    const auto return_value_opt =
        try_execute_internal(pipes, last_return_value);

    if (return_value_opt)
    {
        return {0, return_value_opt};
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        throw std::runtime_error(
            std::format("Fork failed: {}.", std::strerror(errno)));
    }

    if (pid == 0)
    {
        execute_child(pipes);
    }

    return {pid, std::nullopt};
}

const std::optional<std::string>& command::get_input_file() const
{
    return redirections.input_file;
}

const std::optional<std::string>& command::get_output_file() const
{
    return redirections.output_file;
}

bool command::get_output_append() const
{
    return redirections.output_append;
}

std::ostream& operator<<(std::ostream& out, const command& command)
{
    out << "path: " << command.get_path() << "\n"
        << "args:\n";

    for (auto arg : command.get_argv())
    {
        out << "\t" << arg << "\n";
    }

    return out << "input redirection: "
               << command.get_input_file().value_or("(null)") << "\n"
               << "output redirection: "
               << command.get_output_file().value_or("(null)")
               << ", append?: " << std::boolalpha
               << command.get_output_append();
}
}
