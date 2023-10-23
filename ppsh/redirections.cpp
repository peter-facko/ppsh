#include <redirections.hpp>

#include <file_descriptor.hpp>

#include <readline/readline.h>

#include <cassert>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <optional>
#include <unistd.h>

namespace ppsh
{
redirections::redirections()
    : input_file()
    , output_file()
    , output_append(false)
{}

void redirections::register_redirection(redirection redirection)
{
    std::optional<std::string>* to_assign = nullptr;

    switch (redirection.type)
    {
        case redirection_type::INPUT:
            to_assign = &this->input_file;
            break;
        case redirection_type::OUTPUT_TRUNCATE:
            to_assign = &output_file;
            output_append = false;
            break;
        case redirection_type::OUTPUT_APPEND:
            to_assign = &output_file;
            output_append = true;
            break;
    }

    auto temp = std::move(redirection);
    *to_assign = std::move(temp.file_path);
}

void redirections::combine(redirections other)
{
    if (!input_file)
    {
        input_file = std::move(other.input_file);
    }

    if (!output_file)
    {
        output_file = std::move(other.output_file);
        output_append = other.output_append;
    }
}

static auto file_descriptor_construct_open_redirections(
    const std::string& file_path,
    int flags)
{
    return file_descriptor::open(file_path,
                                 flags,
                                 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

static void open_file_and_redirect_fd(const std::string& file_path,
                                      int raw_fd,
                                      int flags)
{
    file_descriptor_construct_open_redirections(file_path, flags)
        .redirect_and_destroy(raw_fd);
}

int redirections::get_append_flag() const
{
    if (output_append)
        return O_APPEND;
    else
        return O_TRUNC;
}

void redirections::redirect_io()
{
    // std::cerr << "Redirecting IO\n";

    if (input_file)
    {
        open_file_and_redirect_fd(*input_file, STDIN_FILENO, O_RDONLY);
    }

    if (output_file)
    {
        open_file_and_redirect_fd(*output_file,
                                  STDOUT_FILENO,
                                  O_CREAT | O_WRONLY | get_append_flag());
    }
}
}
