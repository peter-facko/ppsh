#pragma once
#include "command.h"

typedef STAILQ_HEAD(commands, command) commands_t;

/** Represents a non-empty shell pipeline (sequence of piped commands).
 *
 * Example: command1 | command2 | command3
 */
typedef struct pipeline
{
	commands_t head;
	STAILQ_ENTRY(pipeline) link;
} pipeline_t;

/** Special function. */
void pipeline_construct_move(pipeline_t* pipeline, pipeline_t* other);
/** Special function. */
void pipeline_destroy(pipeline_t* pipeline);
/** Special function. */
bool pipeline_is_valid(const pipeline_t* pipeline);

/** Constructs a valid pipeline with a single 'command'. Moves 'command'. */
int pipeline_construct_move_command(pipeline_t* pipeline, command_t* command);
/** Appends 'command' at the end of the 'pipeline'. Moves 'command'. */
int pipeline_append_move_command(pipeline_t* pipeline, command_t* command);

/** Executes the pipeline, storing the return value in 'return_value'. */
int pipeline_execute(pipeline_t* pipeline, int* return_value);

/** Prints debug info about 'pipeline' into 'out'. */
void pipeline_debug(pipeline_t* pipeline, FILE* out);
