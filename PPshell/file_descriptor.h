#pragma once
#include <fcntl.h>
#include <stdbool.h>

typedef struct file_descriptor
{
	int fd;
} file_descriptor_t;

void file_descriptor_construct(file_descriptor_t* fd);
void file_descriptor_construct_move(file_descriptor_t* fd,
									file_descriptor_t* other);
int file_descriptor_assign_move(file_descriptor_t* fd,
								file_descriptor_t* other);
int file_descriptor_destroy(file_descriptor_t* fd);
bool file_descriptor_is_valid(const file_descriptor_t* fd);

int file_descriptor_construct_pipe(file_descriptor_t* fd_in,
								   file_descriptor_t* fd_out);

void file_descriptor_construct_raw_fd(file_descriptor_t* fd, int raw_fd);
int file_descriptor_construct_open(file_descriptor_t* fd, const char* path,
								   int flags, mode_t mode);

int file_descriptor_redirect_destroy(file_descriptor_t* to_duplicate,
									 int where);
