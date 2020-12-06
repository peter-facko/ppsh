#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>

#include "main.h"
#include "internal_commands.h"
#include "signal_handling.h"

unsigned int line_number;
int return_value;
bool handle_SIGINT;

static void handler_SIGINT(int dummy)
{
	if (handle_SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static char* create_prompt()
{
	const char* name = getenv("USER");
	const char* machine = getenv("NAME");
	const char* path = getenv("PWD");

	size_t name_len = strlen(name);
	size_t machine_len = strlen(machine);
	size_t path_len = strlen(path);

	char* prompt = malloc(name_len + machine_len + path_len + 5);

	strcpy(prompt, name);
	strcpy(prompt + name_len + 1, machine);
	strcpy(prompt + name_len + machine_len + 2, path);

	prompt[name_len] = '@';
	prompt[name_len + machine_len + 1] = ':';
	prompt[name_len + machine_len + path_len + 2] = '$';
	prompt[name_len + machine_len + path_len + 3] = ' ';
	prompt[name_len + machine_len + path_len + 4] = '\0';

	return prompt;
}

int yyparse();

static int parse_file(const char* file_path)
{
	int file = open(file_path, O_RDONLY);

	if (file == -1)
	{
		printf("PPshell: %s: No such file or directory\n", file_path);
		return EXIT_FAILURE;
	}

	dup2(file, STDIN_FILENO);
	close(file);
	int status = yyparse();
	close(STDIN_FILENO);

	return 0;
}

static char* readline_with_prompt()
{
	char* prompt = create_prompt();

	char* line = readline(prompt);

	free(prompt);

	return line;
}

static void parse_line_no_SIGINT(const char* line)
{
	handle_SIGINT = false;
	parse_line(line);
	handle_SIGINT = true;
}

static void parse_interactive()
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
				parse_line_no_SIGINT(line);
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
}

int main(int argc, char** argv)
{
	line_number = 1;
	return_value = 0;
	handle_SIGINT = true;

	internal_commands_init();

	{
		char cwd_buffer[1024];
		getcwd(cwd_buffer, sizeof(cwd_buffer));
		setenv("PWD", cwd_buffer, 1);
	}


	int getopt_result;

	const char* _c_line = NULL;
	int _c_index = 0;

	while ((getopt_result = getopt(argc, argv, "c:")) != -1)
	{
		switch (getopt_result)
		{
		case 'c':
			_c_line = optarg;
			_c_index = optind;
			break;
		case '?':
			fprintf(stderr, "invalid option\n");
			return 1;
		default:
			abort();
		}
	}

	if (_c_index != 0 && _c_index <= optind)
	{
		parse_line(_c_line);
	}
	else if (optind != argc)
	{
		int parse_file_status = parse_file(argv[1]);
		if (parse_file_status != 0)
			return parse_file_status;
	}
	else
	{
		parse_interactive();
	}

	return return_value;
}
