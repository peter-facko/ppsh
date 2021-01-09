#pragma once
#include <fcntl.h>
#include <stdbool.h>

/** Represents an open file descriptor */
typedef struct file_descriptor
{
	int fd;
} file_descriptor_t;

/** Constructs an invalid file descriptor. */
void file_descriptor_construct(file_descriptor_t* fd);
/** Special function. */
void file_descriptor_construct_move(file_descriptor_t* fd,
									file_descriptor_t* other);
/** Special function. */
int file_descriptor_assign_move(file_descriptor_t* fd,
								file_descriptor_t* other);
/** Special function. */
int file_descriptor_destroy(file_descriptor_t* fd);
/** Special function. */
bool file_descriptor_is_valid(const file_descriptor_t* fd);

/** Constructs both 'fd_in' and 'fd_out' with ends of a new unnamed pipe. */
int file_descriptor_construct_pipe(file_descriptor_t* fd_in,
								   file_descriptor_t* fd_out);
/** Constructs 'fd' using `open(path, flags, mode)`. */
int file_descriptor_construct_open(file_descriptor_t* fd, const char* path,
								   int flags, mode_t mode);
/** Redirects 'where' to 'to_duplicate'. Destroys to_duplicate. */
int file_descriptor_redirect_destroy(file_descriptor_t* to_duplicate,
									 int where);
