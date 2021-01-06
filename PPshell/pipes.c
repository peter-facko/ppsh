#include "pipes.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "error_handling.h"
#include "file_descriptor.h"

void pipes_construct(pipes_t* pipes)
{
	file_descriptor_construct(&pipes->in);
	file_descriptor_construct(&pipes->out);
	file_descriptor_construct(&pipes->in_future);
}

static int pipes_shift(pipes_t* pipes)
{
	ERROR_CHECK_INT_NEG_ONE(file_descriptor_destroy(&pipes->out));
	ERROR_CHECK_INT_NEG_ONE(
		file_descriptor_assign_move(&pipes->in, &pipes->in_future));

	return 0;
}

int pipes_create_next(pipes_t* pipes)
{
	ERROR_CHECK_INT_NEG_ONE(pipes_shift(pipes));
	ERROR_CHECK_INT_NEG_ONE(
		file_descriptor_construct_pipe(&pipes->in_future, &pipes->out));

	return 0;
}
int pipes_create_last(pipes_t* pipes)
{
	ERROR_CHECK_INT_NEG_ONE(pipes_shift(pipes));

	return 0;
}

int pipes_destroy(pipes_t* pipes)
{
	ERROR_CHECK_INT_NEG_ONE(file_descriptor_destroy(&pipes->in));
	ERROR_CHECK_INT_NEG_ONE(file_descriptor_destroy(&pipes->out));
	ERROR_CHECK_INT_NEG_ONE(file_descriptor_destroy(&pipes->in_future));

	return 0;
}

int pipes_redirect_and_destroy(pipes_t* pipes)
{
	ERROR_CHECK_INT_NEG_ONE(
		file_descriptor_redirect_destroy(&pipes->in, STDIN_FILENO));

	ERROR_CHECK_INT_NEG_ONE(
		file_descriptor_redirect_destroy(&pipes->out, STDOUT_FILENO));

	ERROR_CHECK_INT_NEG_ONE(file_descriptor_destroy(&pipes->in_future));

	return 0;
}

int pipes_get_in_or_std(pipes_t* pipes)
{
	file_descriptor_t* in = &pipes->in;

	if (!file_descriptor_is_valid(in))
		return STDIN_FILENO;
	else
		return in->fd;
}
int pipes_get_out_or_std(pipes_t* pipes)
{
	file_descriptor_t* out = &pipes->out;

	if (!file_descriptor_is_valid(out))
		return STDOUT_FILENO;
	else
		return out->fd;
}
