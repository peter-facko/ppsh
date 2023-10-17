#include <boost/spirit/home/x3/char/any_char.hpp>
#include <command.hpp>
#include <exceptions.hpp>
#include <pipeline.hpp>
#include <redirection.hpp>
#include <redirections.hpp>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <format>
#include <iostream>
#include <iterator>
#include <ranges>

namespace PPshell
{
static unsigned int line_number;

struct redirection_type_symbols_ : boost::spirit::x3::symbols<redirection_type>
{
    redirection_type_symbols_()
    {
        add(">", redirection_type::OUTPUT_TRUNCATE)(
            ">>",
            redirection_type::OUTPUT_APPEND)("<", redirection_type::INPUT);
    }
} static const redirection_type_symbols;

namespace parser
{
using boost::spirit::x3::_attr;
using boost::spirit::x3::_val;
using boost::spirit::x3::alnum;
using boost::spirit::x3::any_char;
using boost::spirit::x3::char_;
using boost::spirit::x3::eoi;
using boost::spirit::x3::lexeme;
using boost::spirit::x3::lit;
using boost::spirit::x3::rule;
using boost::spirit::x3::skip;
using boost::spirit::x3::space;

const auto newline = char_('\n')[(
    []()
    {
        ++line_number;
    })];

const rule<class token_rule, std::string> token = "token";

const auto token_def =
    lexeme[+(char_("a-zA-Z0-9./") | char_('-') | char_('_'))][(
        [](auto& context)
        {
            std::string token;

            for (auto c : _attr(context))
            {
                token.push_back(get<char>(c));
            }

            _val(context) = std::move(token);
        })];

BOOST_SPIRIT_DEFINE(token);

const rule<class redirection_rule, std::optional<PPshell::redirection>>
    redirection = "redirection";

const auto redirection_def = (redirection_type_symbols >> token)[(
    [](auto& context)
    {
        const auto type = at_c<0>(_attr(context));
        auto& path = at_c<1>(_attr(context));

        _val(context) = PPshell::redirection(std::move(path), type);
    })];

BOOST_SPIRIT_DEFINE(redirection);

const auto argument = token;

const auto command_path = token;

const rule<class redirection_or_argument_rule,
           std::optional<std::variant<PPshell::redirection, std::string>>>
    redirection_or_argument = "redirection_or_argument";

const auto redirection_or_argument_def = (redirection | argument)[(
    [](auto& context)
    {
        auto& redirection_or_argument = _attr(context);

        if (auto* const redirection = get<std::optional<PPshell::redirection>>(
                &redirection_or_argument))
        {
            _val(context) = std::move(*redirection);
        }
        else
        {
            _val(context) =
                get<std::string>(std::move(redirection_or_argument));
        }
    })];

BOOST_SPIRIT_DEFINE(redirection_or_argument);

const rule<class command_rule, std::optional<PPshell::command>> command =
    "command";

const auto command_def =
    (*redirection >> command_path >> *redirection_or_argument)[(
        [](auto& context)
        {
            const auto& redirections_prefix = at_c<0>(_attr(context));
            auto& command_path = at_c<1>(_attr(context));
            const auto& redirections_and_arguments = at_c<2>(_attr(context));

            redirections redirections;

            for (auto& redirection_opt : redirections_prefix)
            {
                redirections.register_redirection(*std::move(redirection_opt));
            }

            PPshell::command command(std::move(command_path));

            for (auto& redirection_or_argument_opt : redirections_and_arguments)
            {
                if (auto* const redirection = std::get_if<PPshell::redirection>(
                        &*redirection_or_argument_opt))
                {
                    redirections.register_redirection(std::move(*redirection));
                }
                else
                {
                    command.register_argument(std::get<std::string>(
                        *std::move(redirection_or_argument_opt)));
                }
            }

            command.register_redirections(std::move(redirections));

            _val(context) = std::move(command);
        })];

BOOST_SPIRIT_DEFINE(command);

const auto pipeline = (command % char_('|'))[(
    [](auto& context)
    {
        auto& commands = _attr(context);

        PPshell::pipeline pipeline(
            std::ranges::views::transform(commands,
                                          [](auto& command_opt) -> auto&&
                                          {
                                              return *std::move(command_opt);
                                          }));

        // std::cerr << pipeline << std::endl;

        static int last_return_value = 0;

        last_return_value = pipeline.execute(last_return_value);
    })];

const auto pipelines = pipeline % char_(';');

const auto main_grammar = -pipelines % newline;

const auto grammar = skip(space - char_('\n'))[main_grammar >> eoi];
}

int process(std::istream& input)
{
    input >> std::noskipws;

    line_number = 1;

    bool r = parse(boost::spirit::istream_iterator(input),
                   boost::spirit::istream_iterator(),
                   parser::grammar);

    if (!r)
    {
        throw exceptions::message_user_exception(
            std::format("Line {}: Failed to parse input.", line_number));
    }

    return 0;
}
}
