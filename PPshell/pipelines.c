#include "pipelines.h"

#include <assert.h>
#include <stdio.h>

#include "error_handling.h"
#include "stailq_helpers.h"

CREATE_STAILQ_HELPERS(pipelines, pipelines_head, head, pipeline, link)

bool pipelines_is_valid(const pipelines_t* pipelines)
{
	return !STAILQ_EMPTY(pipelines_get_head_const(pipelines));
}

void pipelines_construct_move(pipelines_t* pipelines, pipelines_t* other)
{
	assert(pipelines != NULL);
	assert(other != NULL);

	pipelines->head = other->head;
	STAILQ_INIT(pipelines_get_head(other));
}

static int pipelines_append_move_implementation(pipelines_t* pipelines,
												pipeline_t* pipeline)
{
	assert(pipelines != NULL);
	assert(pipeline != NULL);
	assert(pipeline_is_valid(pipeline));

	pipeline_t* allocated_pipeline;
	ERROR_CHECK_PTR_NEG_ONE(allocated_pipeline =
								(pipeline_t*)malloc(sizeof(pipeline_t)));

	pipeline_construct_move(allocated_pipeline, pipeline);

	STAILQ_INSERT_TAIL(pipelines_get_head(pipelines), allocated_pipeline, link);

	return 0;
}

int pipelines_construct_pipeline_move(pipelines_t* pipelines,
									  pipeline_t* pipeline)
{
	assert(pipelines != NULL);
	assert(pipeline != NULL);
	assert(pipeline_is_valid(pipeline));

	STAILQ_INIT(pipelines_get_head(pipelines));

	return pipelines_append_move_implementation(pipelines, pipeline);
}
int pipelines_append_move(pipelines_t* pipelines, pipeline_t* pipeline)
{
	assert(pipelines != NULL);
	assert(pipeline != NULL);
	assert(pipelines_is_valid(pipelines));
	assert(pipeline_is_valid(pipeline));

	return pipelines_append_move_implementation(pipelines, pipeline);
}

void pipelines_destroy(pipelines_t* pipelines)
{
	while (!STAILQ_EMPTY(pipelines_get_head(pipelines)))
		free(pipelines_pop_and_destroy(pipelines));
}

void pipelines_execute_and_destroy(pipelines_t* pipelines, int* return_value)
{
	assert(pipelines != NULL);
	assert(return_value != NULL);
	assert(pipelines_is_valid(pipelines));

	pipeline_t* pipeline;
	STAILQ_FOREACH(pipeline, pipelines_get_head(pipelines), link)
	{
		pipeline_execute(pipeline, return_value);
	}

	pipelines_destroy(pipelines);
}

void pipelines_debug(pipelines_t* pipelines, FILE* out)
{
	assert(pipelines != NULL);
	assert(out != NULL);

	pipeline_t* pipeline;
	STAILQ_FOREACH(pipeline, pipelines_get_head(pipelines), link)
	{
		pipeline_debug(pipeline, out);
		fprintf(out, ";\n");
	}
}
