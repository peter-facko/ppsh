#include <builtins.hpp>
#include <commands.hpp>
#include <exceptions.hpp>
#include <program_options.hpp>

#include <exception>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <tuple>

using namespace PPshell;

std::pair<po::options_description, po::variables_map> parse_cli(int argc,
                                                                char** argv)
{
    po::options_description visible_options_description(
        "Usage: PPshell [options] [file_path]\n\nAllowed options");

    visible_options_description.add_options()(
        "command",
        po::value<std::string>()->value_name("<command>"),
        "Executes the command from CLI.")("help", "Displays usage help.");

    po::options_description hidden_options_description;
    hidden_options_description.add_options()(
        "file",
        po::value<std::filesystem::path>(),
        "Executes the <file_path> as a script.");

    po::options_description options_description;
    options_description.add(visible_options_description)
        .add(hidden_options_description);

    po::positional_options_description positional_options_description;
    positional_options_description.add("file", 1);

    po::variables_map parsed_cli;

    try
    {
        po::store(po::command_line_parser(argc, argv)
                      .options(options_description)
                      .positional(positional_options_description)
                      .run(),
                  parsed_cli);
    }
    catch (const po::error& e)
    {
        throw exceptions::wrapper_user_exception(e);
    }

    po::notify(parsed_cli);

    return std::make_pair(std::move(visible_options_description),
                          std::move(parsed_cli));
}

int dispatch(const po::options_description& visible_options_description,
             const po::variables_map& parsed_cli)
{
    if (parsed_cli.contains("help"))
    {
        return commands::help(std::cout, visible_options_description);
    }
    else if (parsed_cli.contains("command"))
    {
        return commands::command(parsed_cli["command"].as<std::string>());
    }
    else if (parsed_cli.contains("file"))
    {
        return commands::file(parsed_cli["file"].as<std::filesystem::path>());
    }
    else
    {
        return commands::interactive();
    }
}

int main(int argc, char** argv)
{
    try
    {
        const auto [visible_options_description, parsed_cli] =
            parse_cli(argc, argv);

        return dispatch(visible_options_description, parsed_cli);
    }
    catch (const exceptions::user_exception& e)
    {
        std::cerr << "PPshell: " << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "PPshell INTERNAL EXCEPTION: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "PPshell INTERNAL EXCEPTION: unknown\n";
    }

    return 1;
}
