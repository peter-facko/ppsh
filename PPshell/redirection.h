#pragma once
#include "string.h"
#include <stdbool.h>

typedef enum redirection_type
{
	RT_INPUT,
	RT_OUTPUT_APPEND,
	RT_OUTPUT_TRUNCATE,
} redirection_type_t;

typedef struct redirection
{
	string_t file_path;
	redirection_type_t type;
} redirection_t;

void redirection_init_move(redirection_t* red, string_t* file_path, redirection_type_t type);
void redirection_assign_move(redirection_t* old_red, redirection_t* new_red);
void redirection_destroy(redirection_t* red);
