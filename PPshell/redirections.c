#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "redirections.h"
#include "redirect_fd.h"

void redirections_construct(redirections_t* redirections)
{
	string_construct(&redirections->input_file);
	string_construct(&redirections->output_file);
	redirections->output_append = false;
}
void redirections_construct_move(redirections_t* redirections, redirections_t* other)
{
	string_construct_move(&redirections->input_file , &other->input_file);
	string_construct_move(&redirections->output_file, &other->output_file);
	redirections->output_append = other->output_append;
}
void redirections_register_move(redirections_t* redirections, redirection_t* redirection)
{
	string_t* to_assign;

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
void redirections_combine_move(redirections_t* redirections, redirections_t* other)
{
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

static int open_file_and_redirect_fd(string_t* file_path, int to_change_fd, int flags)
{
	if (string_is_valid(file_path))
	{
		const char* file_path_cstr = string_get(file_path);
		
		int new_fd = open(file_path_cstr, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (new_fd == -1)
		{
			fprintf(stderr, "PPshell: %s: couldn't open file\n", file_path_cstr);
			return 1;
		}

		redirect_fd(to_change_fd, new_fd);
	}

	return 0;
}

static int redirections_get_append_flag(redirections_t* redirections)
{
	if (redirections->output_append)
		return O_APPEND;
	else
		return O_TRUNC;
}

int redirections_redirect_io(redirections_t* redirections)
{
	if (open_file_and_redirect_fd(&redirections->input_file, STDIN_FILENO, O_RDONLY) != 0)
		return 1;

	if (open_file_and_redirect_fd(&redirections->output_file, STDOUT_FILENO, O_CREAT | O_WRONLY | redirections_get_append_flag(redirections)) != 0)
		return 1;

	return 0;
}
