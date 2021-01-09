#pragma once
#include <stdbool.h>

#include "string.h"

/** Represents a type of file redirection */
typedef enum redirection_type
{
	RT_INPUT,
	RT_OUTPUT_APPEND,
	RT_OUTPUT_TRUNCATE,
} redirection_type_t;

/** Represents a file redirection in a command.
 *
 * Example: command <redir_input >redir_output_truncate >>redir_output_append
 * 		there are three different redirections in the example command
 */
typedef struct redirection
{
	string_t file_path;
	redirection_type_t type;
} redirection_t;

/** Special function. */
bool redirection_is_valid(const redirection_t* red);
/** Special function. */
void redirection_destroy(redirection_t* red);

/** Constructs a valid redirection of 'file_path' with 'type'. */
void redirection_construct_path_move(redirection_t* red, string_t* file_path,
									 redirection_type_t type);
