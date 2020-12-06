#include "internal_commands.h"
#include <string.h>

internal_command_info_t internal_commands[2];

command_handler_t internal_exit;
command_handler_t internal_cd;

void internal_commands_init()
{
	internal_commands[0].name = "exit";
	internal_commands[0].handler = internal_exit;

	internal_commands[1].name = "cd";
	internal_commands[1].handler = internal_cd;
}

int internal_command_execute(const char* name, strings_t* args, pipe_t pipe, int* return_value)
{
	internal_command_info_t* i = internal_commands;
	const internal_command_info_t* end = internal_commands + (sizeof(internal_commands) / sizeof(internal_command_info_t));

	for (; i != end; ++i)
	{
		if (strcmp(name, i->name) == 0)
		{
			*return_value = i->handler(strings_get_count(args), strings_get(args), pipe);
			return 0;
		}
	}

	return -1;
}
