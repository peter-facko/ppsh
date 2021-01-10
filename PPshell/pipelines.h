#pragma once
#include "pipeline.h"

typedef STAILQ_HEAD(pipelines_head, pipeline) pipelines_head_t;

/** Represents a non-empty sequence of pipelines in shell.
 *
 * Example: pipeline1; pipeline2; pipeline3
 */
typedef struct pipelines
{
	pipelines_head_t head;
} pipelines_t;

/** Special function. */
void pipelines_construct_move(pipelines_t* pipelines, pipelines_t* other);
/** Special function. */
void pipelines_destroy(pipelines_t* pipelines);
/** Special function. */
bool pipelines_is_valid(const pipelines_t* pipelines);

/** Constructs a 'pipelines' with one 'pipeline'. Moves 'pipeline'. */
int pipelines_construct_pipeline_move(pipelines_t* pipelines,
									  pipeline_t* pipeline);

/** Appends 'pipeline' to 'pipelines'. Moves 'pipeline'. */
int pipelines_append_move(pipelines_t* pipelines, pipeline_t* pipeline);

/** Executes 'pipelines', stores its 'return_value' and finally destroys
 * 'pipelines'. */
void pipelines_execute_and_destroy(pipelines_t* pipelines, int* return_value);

/** Prints debug info about 'pipelines' into 'out'. */
void pipelines_debug(pipelines_t* pipelines, FILE* out);
