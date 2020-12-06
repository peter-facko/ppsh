#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pipeline.h"
#include "stailq_helpers.h"
#include "return_value.h"
#include "signal_handling.h"
#include "pipes.h"

static commands_t* pipeline_get_head(pipeline_t* pipeline)
{
	return &pipeline->head;
}

void pipeline_construct_move(pipeline_t* pipeline, pipeline_t* other)
{
	pipeline->head = other->head;
	STAILQ_INIT(pipeline_get_head(other));
}
void pipeline_init_move(pipeline_t* pipeline, command_t* cmd)
{
	STAILQ_INIT(pipeline_get_head(pipeline));

	pipeline_append_command_move(pipeline, cmd);
}

void pipeline_append_command_move(pipeline_t* pipeline, command_t* cmd)
{
	command_t* allocated_cmd = malloc(sizeof(command_t));
	command_construct_move(allocated_cmd, cmd);

	STAILQ_INSERT_TAIL(pipeline_get_head(pipeline), allocated_cmd, link);
}

CREATE_STAILQ_POP_AND_DESTROY(pipeline, command, link)

void pipeline_destroy(pipeline_t* pipeline)
{
	while (!STAILQ_EMPTY(pipeline_get_head(pipeline)))
        free(pipeline_pop_and_destroy(pipeline));
}

static void wait_for_last_child(pid_t id)
{
	if (id != 0)
	{
		int status;

		waitpid(id, &status, 0);
		if (WIFEXITED(status))
		{
			return_value = WEXITSTATUS(status);
		}
		else
		{
			if (WIFSIGNALED(status))
			{
				int child_signal = WTERMSIG(status);
				fprintf(stderr, "Killed by signal %d\n", child_signal);
				return_value = 128 + child_signal;
			}
			else
				return_value = status;
		}
	}
}

static void wait_for_children(size_t children_count)
{
	int status;
	while (children_count != 0)
	{
		wait(&status);
		--children_count;
	}
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

typedef struct pid_and_count
{
	pid_t pid;
	size_t count;
} pid_and_count_t;

static pid_and_count_t pipeline_spawn_children(pipeline_t* pipeline)
{
	pid_and_count_t id_count;

	id_count.count = pipeline_count_commands(pipeline);
	
	pipes_t pipes;

	pipes_construct(&pipes);

	size_t i = 0;

	command_t* cmd;
	STAILQ_FOREACH(cmd, pipeline_get_head(pipeline), link)
	{
		if (i != id_count.count - 1)
			pipes_create_next(&pipes);
		else
			pipes_create_last(&pipes);

		id_count.pid = command_execute(cmd, &pipes);

		++i;
	}

	pipes_destroy(&pipes);

	return id_count;
}

int pipeline_execute(pipeline_t* pipeline)
{
	pid_and_count_t id_count = pipeline_spawn_children(pipeline);

	wait_for_last_child(id_count.pid);

	wait_for_children(id_count.count - 1);
}
