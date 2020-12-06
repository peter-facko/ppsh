#pragma once
#include <stdbool.h>

typedef struct string
{
	char* data;
} string_t;

void string_construct(string_t* string);
void string_construct_move(string_t* string, string_t* new_string);
void string_init_cstring_copy(string_t* string, const char* data);
void string_assign_move(string_t* string, string_t* new_string);
void string_destroy(string_t* string);
bool string_is_valid(string_t* string);
char* string_get(string_t* string);
