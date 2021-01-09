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
	assert(string_is_valid(command_path));

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
	assert(string_is_valid(argument));

	ERROR_CHECK_INT_NEG_ONE(strings_append_move(&command->arguments, argument));

	return 0;
}
void command_register_redirection_move(command_t* command,
									   redirection_t* redirection)
{
	assert(command != NULL);
	assert(redirection != NULL);
	assert(redirection_is_valid(redirection));

	redirections_register_move(&command->redirections, redirection);
}

void command_combine_redirections_move(command_t* command,
									   redirections_t* redirections)
{
	assert(command != NULL);
	assert(redirections != NULL);
	assert(redirections_is_valid(redirections));

	redirections_combine_move(&command->redirections, redirections);
}

const char* command_get_path(command_t* command)
{
	assert(command != NULL);
	assert(command_is_valid(command));

	return string_get(strings_get_begin(&command->arguments));
}
char* const* command_get_arguments(command_t* command)
{
	assert(command != NULL);
	assert(command_is_valid(command));

	return strings_get(&command->arguments);
}

void command_destroy(command_t* command)
{
	assert(command != NULL);

	strings_destroy(&command->arguments);
	redirections_destroy(&command->redirections);
}

bool command_is_valid(const command_t* command)
{
	assert(command != NULL);

	return strings_is_valid(&command->arguments) &&
		   strings_get_count(&command->arguments) >= 1 &&
		   redirections_is_valid(&command->redirections);
}

static int command_redirect_io(command_t* command)
{
	assert(command != NULL);
	assert(command_is_valid(command));

	return redirections_redirect_io(&command->redirections);
}

static internal_command_info_t* find_internal_command_with_name(
	const char* name)
{
	assert(name != NULL);

	internal_command_info_t* i;

	for (i = internal_commands_get_begin();
		 i != internal_commands_get_end() && strcmp(name, i->name) != 0; ++i)
		;

	return i;
}

static bool command_try_execute_internal(command_t* command, pipes_t* pipes,
										 int* return_value)
{
	assert(command != NULL);
	assert(pipes != NULL);
	assert(return_value != NULL);
	assert(command_is_valid(command));

	internal_command_info_t* internal_command =
		find_internal_command_with_name(command_get_path(command));

	if (internal_command == internal_commands_get_end())
		return false;

	strings_t* args = &command->arguments;

	*return_value = internal_command->handler(
		strings_get_count(args), strings_get(args), pipes_get_in(pipes),
		pipes_get_out(pipes), *return_value);

	return true;
}

static void __attribute__((__noreturn__))
command_execute_child(command_t* command, pipes_t* pipes)
{
	assert(command != NULL);
	assert(pipes != NULL);
	assert(command_is_valid(command));

	signal(SIGINT, SIG_DFL);

	pipes_redirect_and_destroy(pipes);

	const char* path = command_get_path(command);

	if (command_redirect_io(command) != 0)
	{
		fprintf(stderr, "%s: couldn't redirect\n", path);
		exit(126);
	}

	execvp(path, command_get_arguments(command));

	fprintf(stderr, "%s: couldn't execute (%s)\n", path, strerror(errno));
	exit(127);
}

pid_t command_execute(command_t* command, pipes_t* pipes, int* return_value)
{
	assert(command != NULL);
	assert(pipes != NULL);
	assert(command_is_valid(command));

	if (command_try_execute_internal(command, pipes, return_value))
		return 0;

	pid_t pid = fork();

	if (pid == -1)
	{
		fprintf(stderr, "Couldn't fork (%s).\n", strerror(errno));
	}

	if (pid == 0)
		command_execute_child(command, pipes);

	return pid;
}

void command_debug(command_t* command, FILE* out)
{
	assert(command != NULL);
	assert(command_is_valid(command));

	fprintf(out, "path: %s\n", command_get_path(command));

	fprintf(out, "args:\n");

	for (char* const* args_begin = command_get_arguments(command) + 1;
		 *args_begin != NULL; ++args_begin)
	{
		fprintf(out, "\t%s\n", *args_begin);
	}

	fprintf(out, "input redirection: %s\n",
			string_get(&command->redirections.input_file));
	fprintf(out, "output redirection: %s, append?: %d\n",
			string_get(&command->redirections.input_file),
			command->redirections.output_append);
}
