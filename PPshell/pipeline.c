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
	assert(pipeline != NULL);

	return &pipeline->head;
}

void pipeline_construct_move(pipeline_t* pipeline, pipeline_t* other)
{
	assert(pipeline != NULL);
	assert(other != NULL);

	pipeline->head = other->head;
	STAILQ_INIT(pipeline_get_head(other));
}
void pipeline_construct_move_command(pipeline_t* pipeline, command_t* cmd)
{
	STAILQ_INIT(pipeline_get_head(pipeline));

	pipeline_append_move_command(pipeline, cmd);
}

void pipeline_append_move_command(pipeline_t* pipeline, command_t* command)
{
	assert(pipeline != NULL);
	assert(command != NULL);
	assert(command_is_valid(command));

	command_t* allocated_command = (command_t*)malloc(sizeof(command_t));
	command_construct_move(allocated_command, command);

	STAILQ_INSERT_TAIL(pipeline_get_head(pipeline), allocated_command, link);
}

CREATE_STAILQ_POP_AND_DESTROY(pipeline, command, link)

void pipeline_destroy(pipeline_t* pipeline)
{
	assert(pipeline != NULL);

	while (!STAILQ_EMPTY(pipeline_get_head(pipeline)))
		free(pipeline_pop_and_destroy(pipeline));
}
bool pipeline_is_valid(pipeline_t* pipeline)
{
	return !STAILQ_EMPTY(pipeline_get_head(pipeline));
}

static int child_status_to_return_value(int status)
{
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	else
	{
		if (WIFSIGNALED(status))
		{
			int child_signal = WTERMSIG(status);
			fprintf(stderr, "Killed by signal %d\n", child_signal);
			return 128 + child_signal;
		}
		else
			return status;
	}
}

static int wait_for_last_child(pid_t id, int* return_value)
{
	assert(id >= 0);
	assert(return_value != NULL);

	if (id == 0)
		return 0;

	int status;

	if (waitpid(id, &status, 0) == (pid_t)-1)
	{
		fprintf(stderr, "wait failed\n");
		return -1;
	}

	*return_value = child_status_to_return_value(status);

	return 0;
}

static int wait_for_children(size_t children_count)
{
	bool fail = false;

	while (children_count != 0)
	{
		if (wait(&(int){0}) == (pid_t)-1 && !fail)
		{
			fprintf(stderr, "wait failed\n");
			fail = true;
		}
		--children_count;
	}

	return fail ? -1 : 0;
}

static int pipes_create_helper(pipes_t* pipes, command_t* command)
{
	// !command_is_last(command)
	if (command->link.stqe_next != NULL)
		return pipes_create_next(pipes);
	else
		return pipes_create_last(pipes);
}

static int pipeline_spawn_children(pipeline_t* pipeline, pid_t* last_child_pid,
								   size_t* spawned, int* return_value)
{
	assert(pipeline != NULL);
	assert(last_child_pid != NULL);
	assert(spawned != NULL);
	assert(return_value != NULL);
	assert(pipeline_is_valid(pipeline));

	pipes_t pipes;

	pipes_construct(&pipes);

	int return_status = 0;

	size_t spawned_children = 0;

	command_t* cmd;
	STAILQ_FOREACH(cmd, pipeline_get_head(pipeline), link)
	{
		if (pipes_create_helper(&pipes, cmd) != 0)
		{
			return_status = -2;
			break;
		}

		*last_child_pid = command_execute(cmd, &pipes, return_value);

		if (*last_child_pid == -1)
		{
			return_status = -3;
			break;
		}

		++spawned_children;
	}

	ERROR_CHECK_INT_NEG_ONE(pipes_destroy(&pipes));

	*spawned = spawned_children;

	return return_status;
}

void pipeline_debug(pipeline_t* pipeline, FILE* out)
{
	assert(pipeline != NULL);
	assert(pipeline_is_valid(pipeline));

	command_t* cmd;
	STAILQ_FOREACH(cmd, pipeline_get_head(pipeline), link)
	{
		command_debug(cmd, out);
		fprintf(out, "|\n");
	}
}

int pipeline_execute(pipeline_t* pipeline, int* return_value)
{
	assert(pipeline != NULL);
	assert(return_value != NULL);
	assert(pipeline_is_valid(pipeline));

	pid_t last_child_pid;
	size_t spawned_children;

	int return_status = 0;

	if (pipeline_spawn_children(pipeline, &last_child_pid, &spawned_children,
								return_value) != 0)
	{
		return_status = -2;
	}
	else if (wait_for_last_child(last_child_pid, return_value) != 0)
	{
		return_status = -3;
	}
	else
	{
		--spawned_children;
	}

	ERROR_CHECK_INT_NEG_ONE(wait_for_children(spawned_children));

	return return_status;
}
