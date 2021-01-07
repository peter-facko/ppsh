#pragma once
#include "pipeline.h"

typedef STAILQ_HEAD(pipelines_head, pipeline) pipelines_head_t;

typedef struct pipelines
{
	pipelines_head_t head;
} pipelines_t;

void pipelines_construct_move(pipelines_t* pipelines, pipelines_t* other);
void pipelines_init_move(pipelines_t* pipelines, pipeline_t* pipeline);
void pipelines_append_move(pipelines_t* pipelines, pipeline_t* pipeline);
void pipelines_destroy(pipelines_t* pipelines);

void pipelines_execute_and_destroy(pipelines_t* pipelines, int* return_value);

void pipelines_debug(pipelines_t* pipelines);
