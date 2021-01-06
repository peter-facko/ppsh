#pragma once
#include <stdbool.h>
#include <stddef.h>

#include "file_descriptor.h"

typedef struct pipes
{
	file_descriptor_t in;
	file_descriptor_t out;
	file_descriptor_t in_future;
} pipes_t;

void pipes_construct(pipes_t* pipes);
int pipes_destroy(pipes_t* pipes);

int pipes_create_next(pipes_t* pipes);
int pipes_create_last(pipes_t* pipes);
int pipes_redirect_and_destroy(pipes_t* pipes);

int pipes_get_in_or_std(pipes_t* pipes);
int pipes_get_out_or_std(pipes_t* pipes);
