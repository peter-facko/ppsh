#include "command.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include "internal_commands.h"
#include "return_value.h"
#include "signal_handling.h"

void command_construct_move(command_t* command, command_t* other)
{
	strings_construct_move(&command->arguments, &other->arguments);
	redirections_construct_move(&command->redirections, &other->redirections);
}

void command_init_move(command_t* command, string_t* command_path)
{
	strings_construct(&command->arguments);
	redirections_construct(&command->redirections);
	strings_append_move(&command->arguments, command_path);
}
void command_register_argument_move(command_t* command, string_t* argument)
{
	strings_append_move(&command->arguments, argument);
}
void command_register_redirection_move(command_t* command, redirection_t* redirection)
{
	redirections_register_move(&command->redirections, redirection);
}

void command_combine_redirections_move(command_t* command, redirections_t* redirections)
{
	redirections_combine_move(&command->redirections, redirections);
}

const char* command_get_path(command_t* command)
{
	return string_get(strings_get_front(&command->arguments));
}
char* const* command_get_arguments(command_t* command)
{
	strings_get(&command->arguments);
}

void command_destroy(command_t* command)
{
	strings_destroy(&command->arguments);
	redirections_destroy(&command->redirections);
}

static int command_redirect_io(command_t* command)
{
	return redirections_redirect_io(&command->redirections);
}

pid_t command_execute(command_t* command, pipes_t* pipes)
{
	if (internal_command_execute(command_get_path(command), &command->arguments, pipes_get(pipes), &return_value) == 0)
		return 0;

	pid_t pid = fork();

	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);

		pipe_t p = pipes_get_and_close(pipes);
		pipe_redirect_io(&p);
		
		int redirection_status = command_redirect_io(command);
		if (redirection_status != 0)
			exit(redirection_status);

		const char* command_path = command_get_path(command);

		if (execvp(command_path, command_get_arguments(command)) == -1)
		{
			fprintf(stderr, "%s: command not found\n", command_path);
			exit(127);
		}
	}

	return pid;
}
