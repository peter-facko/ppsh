#pragma once
#include <stdbool.h>

#include "pipes.h"
#include "redirection.h"
#include "string.h"

typedef struct redirections
{
	string_t output_file;
	string_t input_file;
	bool output_append;
} redirections_t;

void redirections_construct(redirections_t* redirections);
void redirections_construct_move(redirections_t* redirections,
								 redirections_t* other);

void redirections_destroy(redirections_t* redirections);

void redirections_register_move(redirections_t* redirections,
								redirection_t* redirection);
void redirections_combine_move(redirections_t* redirections,
							   redirections_t* other);

int redirections_redirect_io(redirections_t* redirections);
