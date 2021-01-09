#pragma once
#include <stdbool.h>
#include <stddef.h>

#include "file_descriptor.h"

/** Helper structure for creating pipes for pipelines. */
typedef struct pipes
{
	file_descriptor_t in;
	file_descriptor_t out;
	file_descriptor_t in_future;
} pipes_t;

/** Constructs an invalid 'pipes'. */
void pipes_construct(pipes_t* pipes);
/** Special function. */
int pipes_destroy(pipes_t* pipes);

/** Shifts 'pipes' by one and opens a new pipe. */
int pipes_create_next(pipes_t* pipes);
/** Similar to `pipes_create_next`, but doesn't open a new pipe as this is
 * intented to be used before executing the last command in the pipeline. */
int pipes_create_last(pipes_t* pipes);
/** Redirects 'pipes's file descriptors to standard I/O and destroys 'pipes'. */
int pipes_redirect_and_destroy(pipes_t* pipes);

/** Gets the input file descriptor of 'pipes'. */
int pipes_get_in(pipes_t* pipes);
/** Gets the output file descriptor of 'pipes'. */
int pipes_get_out(pipes_t* pipes);
