#pragma once
#include "pipes.h"
#include "strings.h"

typedef int(command_handler_t)(int, char**, int in, int out, int return_value);

typedef struct internal_command_info
{
	const char* name;
	command_handler_t* handler;
} internal_command_info_t;

void internal_commands_init();
internal_command_info_t* internal_commands_get_begin();
internal_command_info_t* internal_commands_get_end();
