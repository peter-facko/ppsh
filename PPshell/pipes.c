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
	assert(pipes != NULL);

	ERROR_CHECK_INT_NEG_ONE(pipes_shift(pipes));
	ERROR_CHECK_INT_NEG_ONE(
		file_descriptor_construct_pipe(&pipes->in_future, &pipes->out));

	return 0;
}
int pipes_create_last(pipes_t* pipes)
{
	assert(pipes != NULL);

	ERROR_CHECK_INT_NEG_ONE(pipes_shift(pipes));

	return 0;
}

int pipes_destroy(pipes_t* pipes)
{
	assert(pipes != NULL);

	ERROR_CHECK_INT_NEG_ONE(file_descriptor_destroy(&pipes->in));
	ERROR_CHECK_INT_NEG_ONE(file_descriptor_destroy(&pipes->out));
	ERROR_CHECK_INT_NEG_ONE(file_descriptor_destroy(&pipes->in_future));

	return 0;
}

int pipes_redirect_and_destroy(pipes_t* pipes)
{
	assert(pipes != NULL);

	ERROR_CHECK_INT_NEG_ONE(
		file_descriptor_redirect_destroy(&pipes->in, STDIN_FILENO));

	ERROR_CHECK_INT_NEG_ONE(
		file_descriptor_redirect_destroy(&pipes->out, STDOUT_FILENO));

	ERROR_CHECK_INT_NEG_ONE(file_descriptor_destroy(&pipes->in_future));

	return 0;
}

static int pipes_get_helper(file_descriptor_t* fd, int if_invalid)
{
	assert(fd != NULL);

	if (!file_descriptor_is_valid(fd))
		return if_invalid;

	return fd->fd;
}

int pipes_get_in(pipes_t* pipes)
{
	return pipes_get_helper(&pipes->in, STDIN_FILENO);
}
int pipes_get_out(pipes_t* pipes)
{
	return pipes_get_helper(&pipes->out, STDOUT_FILENO);
}
