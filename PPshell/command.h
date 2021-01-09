#pragma once
#include <stdio.h>
#include <sys/queue.h>

#include "pipes.h"
#include "redirections.h"
#include "strings.h"

/** Represent a shell command
 *
 * Consists of:
 *      path,
 *      arguments,
 *      redirections
 *
 * Example format:
 * <filei1 >fileo1 command_path arg1 >fileo2 arg2 <filei2
 */
typedef struct command
{
	strings_t arguments;
	redirections_t redirections;
	STAILQ_ENTRY(command) link;
} command_t;

/** Special function. */
void command_construct_move(command_t* command, command_t* other);
/** Special function. */
void command_destroy(command_t* command);
/** Special function. */
bool command_is_valid(const command_t* command);

/** Constructs a command_t with 'command_path', no arguments and no
 * redirections on success.
 *
 * @return Error code.
 * @retval  0 - No error.
 * @retval -1 - Out of memory.
 */
int command_construct_move_cmd_path(command_t* command, string_t* command_path);

/** Appends an 'argument' to 'command'. */
int command_register_argument_move(command_t* command, string_t* argument);
/** Registers a 'redirection' in 'command'. */
void command_register_redirection_move(command_t* command,
									   redirection_t* redirection);

/** Combines 'command's redirections with 'redirections'.
 *
 * The 'command's redirections are changed.
 *
 * 'redirections' are treated as being to the left of 'command's redirections.
 */
void command_combine_redirections_move(command_t* command,
									   redirections_t* redirections);

/** Gets te 'command's path */
const char* command_get_path(command_t* command);
/** Gets the 'command's arguments as a raw null-terminated array of cstrings
 * with the first argument being the 'command's path. */
char* const* command_get_arguments(command_t* command);

/** Executes the command with pipe redirections 'pipes'.
 *
 * Semantically: "pipes->input_pipe | command | pipes->output_pipe".
 */
pid_t command_execute(command_t* command, pipes_t* pipes, int* return_value);

/** Prints debug info about the 'command' into 'out'. */
void command_debug(command_t* command, FILE* out);
