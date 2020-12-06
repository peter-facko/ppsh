#pragma once
#include <stddef.h>

typedef struct pipe
{
	int descriptors[2];
} pipe_t;

int* pipe_get_read(pipe_t* p);
int* pipe_get_write(pipe_t* p);
void pipe_close(pipe_t* p);
void pipe_redirect_io(pipe_t* p);

typedef struct pipes
{
	pipe_t input_pipe;
	pipe_t output_pipe;
} pipes_t;

void pipes_construct(pipes_t* pipes);
pipe_t pipes_get(pipes_t* pipes);
pipe_t pipes_get_and_close(pipes_t* pipes);
void pipes_create_next(pipes_t* pipes);
void pipes_create_last(pipes_t* pipes);
void pipes_destroy(pipes_t* pipes);
