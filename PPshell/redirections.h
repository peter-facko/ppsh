#pragma once
#include <stdbool.h>

#include "pipes.h"
#include "redirection.h"
#include "string.h"

/** Represents all file redirections of a command. */
typedef struct redirections
{
	string_t input_file;
	string_t output_file;
	char output_append;
} redirections_t;

/** Special function. */
void redirections_construct_move(redirections_t* redirections,
								 redirections_t* other);
/** Special function. */
void redirections_destroy(redirections_t* redirections);
/** Special function. */
bool redirections_is_valid(const redirections_t* redirections);
/** Constructs a valid 'redirections'. */
void redirections_construct(redirections_t* redirections);

/** Overrides the 'redirections' in 'redirections'. Moves 'redirection'. */
void redirections_register_move(redirections_t* redirections,
								redirection_t* redirection);
/** Combines 'other' into 'redirections', with 'other' having the higher
 * priority. Moves 'other'.
 */
void redirections_combine_move(redirections_t* redirections,
							   redirections_t* other);

/** Redirects standard I/O to 'redirections'. */
int redirections_redirect_io(redirections_t* redirections);
