#include "file_descriptor.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "error_handling.h"

void file_descriptor_construct(file_descriptor_t* fd)
{
	assert(fd != NULL);

	fd->fd = -1;
}
void file_descriptor_construct_move(file_descriptor_t* fd,
									file_descriptor_t* other)
{
	assert(fd != NULL);
	assert(other != NULL);

	fd->fd = other->fd;
	other->fd = -1;
}
int file_descriptor_assign_move(file_descriptor_t* fd, file_descriptor_t* other)
{
	ERROR_CHECK_INT_NEG_ONE(file_descriptor_destroy(fd));
	file_descriptor_construct_move(fd, other);

	return 0;
}
int file_descriptor_destroy(file_descriptor_t* fd)
{
	if (file_descriptor_is_valid(fd))
	{
		if (close(fd->fd) != 0)
		{
			fprintf(stderr, "close on %d failed (%s)\n", fd->fd,
					strerror(errno));
			return -1;
		}
	}

	fd->fd = -1;

	return 0;
}
bool file_descriptor_is_valid(const file_descriptor_t* fd)
{
	return fd->fd != -1;
}

int file_descriptor_construct_pipe(file_descriptor_t* fd_in,
								   file_descriptor_t* fd_out)
{
	int temp[2];

	if (pipe(temp) != 0)
	{
		file_descriptor_construct(fd_in);
		file_descriptor_construct(fd_out);

		return -1;
	}

	file_descriptor_construct_raw_fd(fd_in, temp[0]);
	file_descriptor_construct_raw_fd(fd_out, temp[1]);

	return 0;
}

void file_descriptor_construct_raw_fd(file_descriptor_t* fd, int raw_fd)
{
	assert(fd != NULL);

	fd->fd = raw_fd;
}

int file_descriptor_construct_open(file_descriptor_t* fd, const char* path,
								   int flags, mode_t mode)
{
	assert(fd != NULL);
	assert(path != NULL);

	int raw_fd = open(path, flags, mode);

	ERROR_CHECK(raw_fd < 0, -1);

	file_descriptor_construct_raw_fd(fd, raw_fd);

	return 0;
}

int file_descriptor_redirect_destroy(file_descriptor_t* to_duplicate, int where)
{
	assert(where >= 0);

	if (file_descriptor_is_valid(to_duplicate) && to_duplicate->fd != where)
	{
		ERROR_CHECK(dup2(to_duplicate->fd, where) == -1, -1);

		ERROR_CHECK_INT_NEG_ONE(file_descriptor_destroy(to_duplicate));
	}

	return 0;
}
