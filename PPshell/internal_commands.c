#include "internal_commands.h"

#include <string.h>

static internal_command_info_t internal_commands[2];

command_handler_t internal_exit;
command_handler_t internal_cd;

void internal_commands_init()
{
	internal_commands[0].name = "exit";
	internal_commands[0].handler = internal_exit;

	internal_commands[1].name = "cd";
	internal_commands[1].handler = internal_cd;
}

internal_command_info_t* internal_commands_get_begin()
{
	return internal_commands;
}
internal_command_info_t* internal_commands_get_end()
{
	return internal_commands_get_begin() +
		   (sizeof(internal_commands) / sizeof(internal_command_info_t));
}
