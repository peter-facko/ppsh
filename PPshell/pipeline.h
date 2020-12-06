#pragma once
#include "command.h"

typedef STAILQ_HEAD(commands, command) commands_t;

typedef struct pipeline
{
	commands_t head;
	STAILQ_ENTRY(pipeline) link;
} pipeline_t;

void pipeline_construct_move(pipeline_t* pipeline, pipeline_t* other);
void pipeline_init_move(pipeline_t* pipeline, command_t* command);
void pipeline_append_command_move(pipeline_t* pipeline, command_t* command);
void pipeline_destroy(pipeline_t* pipeline);

int pipeline_execute(pipeline_t* pipeline);
