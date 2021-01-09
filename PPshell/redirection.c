#include "redirection.h"

#include <assert.h>
#include <stddef.h>

static bool redirection_type_is_valid(const redirection_type_t* type)
{
	return *type == RT_INPUT || *type == RT_OUTPUT_APPEND ||
		   *type == RT_OUTPUT_TRUNCATE;
}
static void redirection_type_destroy(redirection_type_t* type)
{
	*type = (redirection_type_t)3;
}
static void redirection_type_construct_move(redirection_type_t* type,
											redirection_type_t* other)
{
	*type = *other;
	redirection_type_destroy(other);
}

void redirection_construct_path_move(redirection_t* red, string_t* file_path,
									 redirection_type_t type)
{
	assert(red != NULL);
	assert(file_path != NULL);
	assert(redirection_type_is_valid(&type));
	assert(string_is_valid(file_path));

	string_construct_move(&red->file_path, file_path);
	redirection_type_construct_move(&red->type, &type);
}

void redirection_destroy(redirection_t* red)
{
	redirection_type_destroy(&red->type);
	string_destroy(&red->file_path);
}

bool redirection_is_valid(const redirection_t* red)
{
	return string_is_valid(&red->file_path) &&
		   redirection_type_is_valid(&red->type);
}
