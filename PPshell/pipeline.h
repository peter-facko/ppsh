#pragma once
#include "command.h"

typedef STAILQ_HEAD(commands, command) commands_t;

/** Represents a shell pipeline. */
typedef struct pipeline
{
	commands_t head;
	STAILQ_ENTRY(pipeline) link;
} pipeline_t;

/** Constructs a valid pipeline. */
void pipeline_construct_move(pipeline_t* pipeline, pipeline_t* other);
void pipeline_destroy(pipeline_t* pipeline);

/** Constructs a valid pipeline with a single 'command'. Moves 'command'. */
void pipeline_construct_move_command(pipeline_t* pipeline, command_t* command);
/** Appends 'command' at the end of the 'pipeline'. Moves 'command'. */
void pipeline_append_move_command(pipeline_t* pipeline, command_t* command);

/** Executes the pipeline, storing the return value in 'return_value'. */
int pipeline_execute(pipeline_t* pipeline, int* return_value);

/** Prints debug info aboud 'pipeline'. */
void pipeline_debug(pipeline_t* pipeline);
