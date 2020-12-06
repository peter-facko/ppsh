#pragma once
#include <sys/queue.h>

#include "strings.h"
#include "redirections.h"
#include "pipes.h"

typedef struct command
{
	strings_t arguments;
	redirections_t redirections;
	STAILQ_ENTRY(command) link;
} command_t;

void command_construct_move(command_t* command, command_t* other);
void command_init_move(command_t* command, string_t* command_path);

void command_register_argument_move(command_t* command, string_t* argument);
void command_register_redirection_move(command_t* command, redirection_t* redirection);
void command_combine_redirections_move(command_t* command, redirections_t* redirection);

void command_destroy(command_t* command);

const char* command_get_path(command_t* command);
char* const* command_get_arguments(command_t* command);

pid_t command_execute(command_t* command, pipes_t* pipes);
