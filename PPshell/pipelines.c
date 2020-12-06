#include "pipelines.h"
#include "stailq_helpers.h"
#include "return_value.h"

static pipelines_head_t* pipelines_get_head(pipelines_t* pipelines)
{
	return &pipelines->head;
}

void pipelines_construct_move(pipelines_t* pipelines, pipelines_t* other)
{
	pipelines->head = other->head;
	STAILQ_INIT(pipelines_get_head(other));
}
void pipelines_init_move(pipelines_t* pipelines, pipeline_t* pipeline)
{
	STAILQ_INIT(pipelines_get_head(pipelines));

	pipelines_append_move(pipelines, pipeline);
}
void pipelines_append_move(pipelines_t* pipelines, pipeline_t* pipeline)
{
	pipeline_t* allocated_pipeline = malloc(sizeof(pipeline_t));
	pipeline_construct_move(allocated_pipeline, pipeline);

	STAILQ_INSERT_TAIL(pipelines_get_head(pipelines), allocated_pipeline, link);
}

CREATE_STAILQ_POP_AND_DESTROY(pipelines, pipeline, link)

void pipelines_destroy(pipelines_t* pipelines)
{
	while (!STAILQ_EMPTY(pipelines_get_head(pipelines)))
		free(pipelines_pop_and_destroy(pipelines));
}

void pipelines_execute_and_destroy(pipelines_t* pipelines)
{
	pipeline_t* pipeline;
	STAILQ_FOREACH(pipeline, pipelines_get_head(pipelines), link)
	{
		pipeline_execute(pipeline);
	}

	pipelines_destroy(pipelines);
}
