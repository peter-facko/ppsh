#include "command.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "error_handling.h"
#include "internal_commands.h"

void command_construct_move(command_t* command, command_t* other)
{
	assert(command != NULL);
	assert(other != NULL);

	strings_construct_move(&command->arguments, &other->arguments);
	redirections_construct_move(&command->redirections, &other->redirections);
}

int command_construct_move_cmd_path(command_t* command, string_t* command_path)
{
	assert(command != NULL);
	assert(command_path != NULL);

	ERROR_CHECK_INT_NEG_ONE(strings_construct(&command->arguments));
	redirections_construct(&command->redirections);
	ERROR_CHECK_INT_NEG_ONE(
		strings_append_move(&command->arguments, command_path));

	return 0;
}
int command_register_argument_move(command_t* command, string_t* argument)
{
	assert(command != NULL);
	assert(argument != NULL);

	ERROR_CHECK_INT_NEG_ONE(strings_append_move(&command->arguments, argument));

	return 0;
}
void command_register_redirection_move(command_t* command,
									   redirection_t* redirection)
{
	assert(command != NULL);
	assert(redirection != NULL);

	redirections_register_move(&command->redirections, redirection);
}

void command_combine_redirections_move(command_t* command,
									   redirections_t* redirections)
{
	assert(command != NULL);
	assert(redirections != NULL);

	redirections_combine_move(&command->redirections, redirections);
}

const char* command_get_path(command_t* command)
{
	assert(command != NULL);

	string_t* begin = strings_get_front(&command->arguments);

	assert(begin != NULL);

	return string_get(begin);
}
char* const* command_get_arguments(command_t* command)
{
	assert(command != NULL);

	return strings_get(&command->arguments);
}

void command_destroy(command_t* command)
{
	assert(command != NULL);

	strings_destroy(&command->arguments);
	redirections_destroy(&command->redirections);
}

static int command_redirect_io(command_t* command)
{
	assert(command != NULL);

	return redirections_redirect_io(&command->redirections);
}

static int command_try_execute_internal(command_t* command, pipes_t* pipes,
										int* return_value)
{
	assert(command != NULL);
	assert(pipes != NULL);

	strings_t* args = &command->arguments;
	const char* path = command_get_path(command);

	for (internal_command_info_t* i = internal_commands_get_begin();
		 i != internal_commands_get_end(); ++i)
	{
		if (strcmp(path, i->name) == 0)
		{
			*return_value =
				i->handler(strings_get_count(args), strings_get(args),
						   pipes_get_in_or_std(pipes),
						   pipes_get_out_or_std(pipes), *return_value);
			return 0;
		}
	}

	return -1;
}

static void command_execute_child(command_t* command, pipes_t* pipes)
{
	signal(SIGINT, SIG_DFL);

	pipes_redirect_and_destroy(pipes);

	const char* path = command_get_path(command);

	assert(path != NULL);

	if (command_redirect_io(command) != 0)
	{
		fprintf(stderr, "%s: couldn't redirect\n", path);
		exit(126);
	}

	if (execvp(path, command_get_arguments(command)) == -1)
	{
		fprintf(stderr, "%s: couldn't execute (%s)\n", path, strerror(errno));
		exit(127);
	}
}

pid_t command_execute(command_t* command, pipes_t* pipes, int* return_value)
{
	assert(command != NULL);
	assert(pipes != NULL);

	if (command_try_execute_internal(command, pipes, return_value) == 0)
		return 0;

	pid_t pid = fork();

	if (pid == 0)
		command_execute_child(command, pipes);

	return pid;
}

void command_debug(command_t* command)
{
	printf("path: %s\n", command_get_path(command));

	printf("args:\n");

	for (char* const* args_begin = command_get_arguments(command) + 1;
		 *args_begin != NULL; ++args_begin)
	{
		printf("\t%s\n", *args_begin);
	}

	printf("input redirection: %s\n",
		   string_get(&command->redirections.input_file));
	printf("output redirection: %s, append?: %d\n",
		   string_get(&command->redirections.input_file),
		   command->redirections.output_append);
}
