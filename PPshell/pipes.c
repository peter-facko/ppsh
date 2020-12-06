#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "pipes.h"
#include "redirect_fd.h"

static void pipe_create(pipe_t* p)
{
	pipe(p->descriptors);
}

int* pipe_get_read(pipe_t* p)
{
	return &p->descriptors[0];
}
int* pipe_get_write(pipe_t* p)
{
	return &p->descriptors[1];
}
static int pipe_checked_close(int* fd)
{
	if (*fd != -1)
		return close(*fd);

	return 0;
}
static void pipe_close_read(pipe_t* p)
{
	int* in = pipe_get_read(p);
	pipe_checked_close(in);
	*in = -1;
}
static void pipe_close_write(pipe_t* p)
{
	int* out = pipe_get_write(p);
	pipe_checked_close(out);
	*out = -1;
}
void pipe_close(pipe_t* p)
{
	pipe_close_read(p);
	pipe_close_write(p);
}

void pipe_set(pipe_t* pipe, int in, int out)
{
	pipe->descriptors[0] = in;
	pipe->descriptors[1] = out;
}

void pipe_redirect_io(pipe_t* p)
{
	redirect_fd(STDIN_FILENO, *pipe_get_read(p));
	redirect_fd(STDOUT_FILENO, *pipe_get_write(p));
}

void pipes_construct(pipes_t* pipes)
{
	pipe_set(&pipes->input_pipe, -1, -1);
	pipe_set(&pipes->output_pipe, -1, -1);
}

static int pipe_get_read_or_std(pipe_t* pipe)
{
	int in = *pipe_get_read(pipe);
	if (in == -1)
		in = STDIN_FILENO;

	return in;
}

static int pipe_get_write_or_std(pipe_t* pipe)
{
	int out = *pipe_get_write(pipe);
	if (out == -1)
		out = STDOUT_FILENO;

	return out;
}

pipe_t pipes_get(pipes_t* pipes)
{
	pipe_t p;

	pipe_set(&p, pipe_get_read_or_std(&pipes->input_pipe), pipe_get_write_or_std(&pipes->output_pipe));

	return p;
}

pipe_t pipes_get_and_close(pipes_t* pipes)
{
	pipe_close_write(&pipes->input_pipe);
	pipe_close_read(&pipes->output_pipe);

	return pipes_get(pipes);
}

void pipes_shift(pipes_t* pipes)
{
	pipe_close(&pipes->input_pipe);
	pipes->input_pipe = pipes->output_pipe;
}

void pipes_create_next(pipes_t* pipes)
{
	pipes_shift(pipes);

	pipe_create(&pipes->output_pipe);
}
void pipes_create_last(pipes_t* pipes)
{
	pipes_shift(pipes);
	pipe_set(&pipes->output_pipe, -1, -1);
}

void pipes_destroy(pipes_t* pipes)
{
	pipe_close(&pipes->input_pipe);
	pipe_close(&pipes->output_pipe);
}
