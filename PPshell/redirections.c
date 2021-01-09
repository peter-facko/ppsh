#include "redirections.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "error_handling.h"

#define OUTPUT_APPEND_FALSE (0)
#define OUTPUT_APPEND_TRUE (1)
#define OUTPUT_APPEND_INVALID (3)

void redirections_construct(redirections_t* redirections)
{
	string_construct(&redirections->input_file);
	string_construct(&redirections->output_file);
	redirections->output_append = false;
}
void redirections_construct_move(redirections_t* redirections,
								 redirections_t* other)
{
	string_construct_move(&redirections->input_file, &other->input_file);
	string_construct_move(&redirections->output_file, &other->output_file);
	redirections->output_append = other->output_append;

	other->output_append = OUTPUT_APPEND_INVALID;
}
void redirections_register_move(redirections_t* redirections,
								redirection_t* redirection)
{
	assert(redirections != NULL);
	assert(redirection != NULL);
	assert(redirection_is_valid(redirection));

	string_t* to_assign = NULL;

	switch (redirection->type)
	{
	case RT_INPUT:
		to_assign = &redirections->input_file;
		break;
	case RT_OUTPUT_TRUNCATE:
		to_assign = &redirections->output_file;
		redirections->output_append = false;
		break;
	case RT_OUTPUT_APPEND:
		to_assign = &redirections->output_file;
		redirections->output_append = true;
		break;
	}

	string_assign_move(to_assign, &redirection->file_path);
}
void redirections_combine_move(redirections_t* redirections,
							   redirections_t* other)
{
	assert(redirections != NULL);
	assert(other != NULL);

	if (!string_is_valid(&redirections->input_file))
		string_assign_move(&redirections->input_file, &other->input_file);

	if (!string_is_valid(&redirections->output_file))
	{
		string_assign_move(&redirections->output_file, &other->output_file);
		redirections->output_append = other->output_append;
	}

	redirections_destroy(other);
}
void redirections_destroy(redirections_t* redirections)
{
	string_destroy(&redirections->input_file);
	string_destroy(&redirections->output_file);
}

bool redirections_is_valid(const redirections_t* redirections)
{
	return redirections->output_append != OUTPUT_APPEND_INVALID;
}

static int file_descriptor_construct_open_redirections(file_descriptor_t* fd,
													   string_t* file_path,
													   int flags)
{
	assert(fd != NULL);
	assert(file_path != NULL);

	const char* path = string_get(file_path);

	assert(path != NULL);

	if (file_descriptor_construct_open(
			fd, path, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) != 0)
	{
		fprintf(stderr, "PPshell: %s: couldn't open file\n", path);

		return -1;
	}

	return 0;
}

static int open_file_and_redirect_fd(string_t* file_path, int raw_fd, int flags)
{
	file_descriptor_t file;
	int status =
		file_descriptor_construct_open_redirections(&file, file_path, flags);

	ERROR_CHECK_INT(status, status);

	ERROR_CHECK_INT(file_descriptor_redirect_destroy(&file, raw_fd), -2);

	return 0;
}

static int redirections_get_append_flag(const redirections_t* redirections)
{
	if (redirections->output_append)
		return O_APPEND;
	else
		return O_TRUNC;
}

int redirections_redirect_io(redirections_t* redirections)
{
	string_t* in = &redirections->input_file;

	if (string_is_valid(in))
		ERROR_CHECK_INT_NEG_ONE(
			open_file_and_redirect_fd(in, STDIN_FILENO, O_RDONLY));

	string_t* out = &redirections->output_file;

	if (string_is_valid(out))
		ERROR_CHECK_INT_NEG_ONE(open_file_and_redirect_fd(
			out, STDOUT_FILENO,
			O_CREAT | O_WRONLY | redirections_get_append_flag(redirections)));

	return 0;
}
