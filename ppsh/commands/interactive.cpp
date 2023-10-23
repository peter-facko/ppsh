#include <commands.hpp>
#include <cstdio>
#include <process.hpp>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <readline/history.h>
#include <readline/readline.h>

#include <unistd.h>

#include <algorithm>
#include <csignal>
#include <cstdarg>
#include <cstring>
#include <iostream>
#include <span>
#include <string>

static bool handle_SIGINT;
static void handler_SIGINT(int)
{
    if (handle_SIGINT)
    {
        putchar('\n');
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

static const char* create_prompt()
{
    return "$ ";
}

static auto readline_with_prompt()
{
    return std::unique_ptr<char, decltype([](char* ptr){ free(ptr); })>(readline(create_prompt()));
}

static auto process_no_SIGINT(std::istream& input)
{
    handle_SIGINT = false;
    const auto result = ppsh::process(input);
    handle_SIGINT = true;

    return result;
}

int ppsh::commands::interactive()
{
    std::signal(SIGINT, handler_SIGINT);

    rl_bind_key('\t', rl_complete);

    auto last_return_value = 0;

    while (true)
    {
        const auto line_lifetime = readline_with_prompt();
        const char* line = line_lifetime.get();

        if (!line)
        {
            std::cout << "exit\n";
            break;
        }

        if (std::string_view(line) == "")
        {
            continue;
        }

        boost::iostreams::array_source stream_source(line, std::strlen(line));
        boost::iostreams::stream stream(stream_source);

        last_return_value = process_no_SIGINT(stream);

        add_history(line);
    }

    return last_return_value;
}
