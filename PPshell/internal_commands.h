#pragma once
#include "pipes.h"
#include "strings.h"

typedef int(command_handler_t)(int, char**, int in, int out, int return_value);

/** Represents a command which is internal to the shell. */
typedef struct internal_command_info
{
	const char* name;
	command_handler_t* handler;
} internal_command_info_t;

/** Initializes the internal commands. Needs to be run once before other
 * functions using internal commands. */
void internal_commands_init();
/** Gets a pointer to the first internal command. */
internal_command_info_t* internal_commands_get_begin();
/** Gets a pointer *after* the last internal command. */
internal_command_info_t* internal_commands_get_end();
