#pragma once
#include "strings.h"
#include "pipes.h"

typedef int(command_handler_t)(int, char**, pipe_t);

typedef struct internal_command_info
{
	const char* name;
	command_handler_t* handler;
} internal_command_info_t;

extern internal_command_info_t internal_commands[2];

void internal_commands_init();

int internal_command_execute(const char* name, strings_t* args, pipe_t pipe, int* return_value);
