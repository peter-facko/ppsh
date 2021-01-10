#include "main.h"

#include <fcntl.h>
#include <getopt.h>
#include <pwd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "file_descriptor.h"
#include "internal_commands.h"
#include "pipelines.h"

static unsigned int line_number;
static bool handle_SIGINT;

void line_number_increment()
{
	++line_number;
}

static void handler_SIGINT(int signum)
{
	if (handle_SIGINT)
	{
		putchar('\n');
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}

	(void)signum;
}

static char* try_strcpy(char* begin, const char* end, ...)
{
	typedef int string_copier(char*, size_t);

	va_list args;
	va_start(args, end);

	string_copier* copier;

	size_t max_length = end - begin;

	while (true)
	{
		copier = va_arg(args, string_copier*);

		if (copier == NULL || copier(begin, max_length) == 0)
			break;
	}

	va_end(args);

	return begin + strlen(begin);
}

#define CREATE_LITERAL_GETENV(string_literal, suffix)                          \
	static int getenv_##suffix(char* begin, size_t max_length)                 \
	{                                                                          \
		const char* name = getenv(string_literal);                             \
		if (name == NULL)                                                      \
			return -1;                                                         \
                                                                               \
		strncpy(begin, name, max_length);                                      \
		return 0;                                                              \
	}

CREATE_LITERAL_GETENV("USER", USER)
CREATE_LITERAL_GETENV("LOGNAME", LOGNAME)
CREATE_LITERAL_GETENV("NAME", NAME)
CREATE_LITERAL_GETENV("PWD", PWD)

static int getcwd_helper(char* begin, size_t max_length)
{
	char* x = getcwd(begin, max_length);
	return x == NULL ? -1 : 0;
}

#define CREATE_LITERAL_STRNCPY(string_literal, suffix)                         \
	static int strncpy_##suffix(char* begin, size_t max_length)                \
	{                                                                          \
		strncpy(begin, string_literal, max_length);                            \
		return 0;                                                              \
	}

CREATE_LITERAL_STRNCPY("user", USER)
CREATE_LITERAL_STRNCPY("machine", MACHINE)
CREATE_LITERAL_STRNCPY("directory", DIRECTORY)

static char* write_char(char* begin, const char* end, char c)
{
	if (begin != end)
		*begin++ = c;

	return begin;
}

static char* create_prompt()
{
	static char buffer[128] = {0};
	char* i = buffer;
	const char* end = buffer + sizeof(buffer) - 1;

	i = try_strcpy(i, end, getlogin_r, getenv_USER, getenv_LOGNAME,
				   strncpy_USER, NULL);
	i = write_char(i, end, '@');
	i = try_strcpy(i, end, gethostname, getenv_NAME, strncpy_MACHINE, NULL);
	i = write_char(i, end, ':');
	i = try_strcpy(i, end, getcwd_helper, getenv_PWD, strncpy_DIRECTORY, NULL);
	i = write_char(i, end, '$');
	i = write_char(i, end, ' ');

	return buffer;
}

static int parse_file(const char* file_path, int* return_value)
{
	file_descriptor_t file;

	if (file_descriptor_construct_open(&file, file_path, O_RDONLY, 0) != 0)
	{
		fprintf(stderr, "PPshell: %s: Couldn't open.\n", file_path);
		return -1;
	}

	int status = -2;

	if (file_descriptor_redirect_destroy(&file, STDIN_FILENO) == 0)
		status = yyparse(return_value);

	file_descriptor_destroy(&file);

	return status;
}

static char* readline_with_prompt()
{
	return readline(create_prompt());
}

static int parse_line_no_SIGINT(const char* line, int* return_value)
{
	handle_SIGINT = false;
	int status = parse_line(line, return_value);
	handle_SIGINT = true;

	return status;
}

static int parse_interactive(int* return_value)
{
	signal(SIGINT, handler_SIGINT);

	rl_bind_key('\t', rl_complete);

	char* line = NULL;

	while (true)
	{
		line = readline_with_prompt();

		if (line != NULL)
		{
			if (strcmp(line, "") != 0)
			{
				if (parse_line_no_SIGINT(line, return_value) != 0)
				{
					free(line);
					return -1;
				}
				add_history(line);
			}
		}
		else
		{
			printf("exit\n");
			break;
		}

		free(line);
	}

	free(line);

	return 0;
}

int handle_options(int argc, char** argv, const char** c_line, int* c_index)
{
	int getopt_result;

	*c_line = NULL;
	*c_index = 0;

	while ((getopt_result = getopt(argc, argv, "c:")) != -1)
	{
		switch (getopt_result)
		{
		case 'c':
			*c_line = optarg;
			*c_index = optind;
			break;
		case '?':
			fprintf(stderr, "invalid option\n");
			return -1;
		default:
			abort();
		}
	}

	return 0;
}

int main(int argc, char** argv)
{
	line_number = 1;
	int return_value = 0;
	handle_SIGINT = true;

	internal_commands_init();

	const char* c_line;
	int c_index;
	if (handle_options(argc, argv, &c_line, &c_index) != 0)
		return 1;

	if (c_index != 0 && c_index <= optind)
	{
		parse_line(c_line, &return_value);
	}
	else if (optind != argc)
	{
		int parse_file_status = parse_file(argv[1], &return_value);
		if (parse_file_status != 0)
			return parse_file_status;
	}
	else
	{
		int parse_interactive_status = parse_interactive(&return_value);
		if (parse_interactive_status != 0)
			return parse_interactive_status;
	}

	return return_value;
}

void yyerror(int* return_value, const char* message)
{
	fprintf(stderr, "line %d: %s\n", line_number, message);
	*return_value = 2;
}
