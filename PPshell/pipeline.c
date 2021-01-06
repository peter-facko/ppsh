#include "pipeline.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "error_handling.h"
#include "pipes.h"
#include "stailq_helpers.h"

static commands_t* pipeline_get_head(pipeline_t* pipeline)
{
	return &pipeline->head;
}

void pipeline_construct_move(pipeline_t* pipeline, pipeline_t* other)
{
	pipeline->head = other->head;
	STAILQ_INIT(pipeline_get_head(other));
}
void pipeline_construct_move_command(pipeline_t* pipeline, command_t* cmd)
{
	STAILQ_INIT(pipeline_get_head(pipeline));

	pipeline_append_move_command(pipeline, cmd);
}

void pipeline_append_move_command(pipeline_t* pipeline, command_t* cmd)
{
	command_t* allocated_cmd = (command_t*)malloc(sizeof(command_t));
	command_construct_move(allocated_cmd, cmd);

	STAILQ_INSERT_TAIL(pipeline_get_head(pipeline), allocated_cmd, link);
}

CREATE_STAILQ_POP_AND_DESTROY(pipeline, command, link)

void pipeline_destroy(pipeline_t* pipeline)
{
	while (!STAILQ_EMPTY(pipeline_get_head(pipeline)))
		free(pipeline_pop_and_destroy(pipeline));
}

static int wait_for_last_child(pid_t id, int* return_value)
{
	if (id != 0)
	{
		int status;

		if (waitpid(id, &status, 0) == (pid_t)-1)
		{
			fprintf(stderr, "wait failed\n");
			return -1;
		}

		if (WIFEXITED(status))
		{
			*return_value = WEXITSTATUS(status);
		}
		else
		{
			if (WIFSIGNALED(status))
			{
				int child_signal = WTERMSIG(status);
				fprintf(stderr, "Killed by signal %d\n", child_signal);
				*return_value = 128 + child_signal;
			}
			else
				*return_value = status;
		}
	}

	return 0;
}

static int wait_for_children(size_t children_count)
{
	int status;
	while (children_count != 0)
	{
		if (wait(&status) == (pid_t)-1)
		{
			fprintf(stderr, "wait failed\n");
			return -1;
		}
		--children_count;
	}

	return 0;
}

static size_t pipeline_count_commands(pipeline_t* pipeline)
{
	size_t children_count = 0;

	command_t* cmd;
	STAILQ_FOREACH(cmd, pipeline_get_head(pipeline), link)
	{
		++children_count;
	}

	return children_count;
}

static int pipeline_spawn_children(pipeline_t* pipeline, pid_t* last_child_pid,
								   size_t* command_count, int* return_value)
{
	*command_count = pipeline_count_commands(pipeline);

	pipes_t pipes;

	pipes_construct(&pipes);

	size_t i = 0;

	command_t* cmd;
	STAILQ_FOREACH(cmd, pipeline_get_head(pipeline), link)
	{
		int status;

		if (i != *command_count - 1)
			status = pipes_create_next(&pipes);
		else
			status = pipes_create_last(&pipes);

		if (status != 0)
		{
			fprintf(stderr, "couldn't create pipe\n");
			break;
		}

		*last_child_pid = command_execute(cmd, &pipes, return_value);

		++i;
	}

	if (pipes_destroy(&pipes) != 0)
	{
		fprintf(stderr, "couldn't destroy pipes\n");
		return -1;
	}

	return 0;
}

void pipeline_debug(pipeline_t* pipeline)
{
	command_t* cmd;
	STAILQ_FOREACH(cmd, pipeline_get_head(pipeline), link)
	{
		command_debug(cmd);
		printf("|\n");
	}
}

int pipeline_execute(pipeline_t* pipeline, int* return_value)
{
	pid_t last_child_pid;
	size_t command_count;

	ERROR_CHECK_INT_NEG_ONE(pipeline_spawn_children(
		pipeline, &last_child_pid, &command_count, return_value));

	ERROR_CHECK_INT_NEG_ONE(wait_for_last_child(last_child_pid, return_value));

	ERROR_CHECK_INT_NEG_ONE(wait_for_children(command_count - 1));

	return 0;
}
