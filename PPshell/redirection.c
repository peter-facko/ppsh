#include "redirection.h"

void redirection_init_move(redirection_t* red, string_t* file_path,
						   redirection_type_t type)
{
	string_construct_move(&red->file_path, file_path);
	red->type = type;
}

void redirection_destroy(redirection_t* red)
{
	string_destroy(&red->file_path);
}

void redirection_assign_move(redirection_t* old_red, redirection_t* new_red)
{
	redirection_destroy(old_red);
	redirection_init_move(old_red, &new_red->file_path, new_red->type);
}
