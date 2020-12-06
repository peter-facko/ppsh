#include <stdlib.h>
#include <string.h>
#include "string.h"

void string_construct(string_t* string)
{
	string->data = NULL;
}
void string_construct_move(string_t* string, string_t* new_string)
{
	string->data = new_string->data;
	new_string->data = NULL;
}
void string_init_cstring_copy(string_t* string, const char* data)
{
	string->data = strdup(data);
}
void string_assign_move(string_t* string, string_t* new_string)
{
	string_destroy(string);
	string_construct_move(string, new_string);
}

void string_destroy(string_t* string)
{
	free(string->data);
	string->data = NULL;
}

bool string_is_valid(string_t* string)
{
	return string->data != NULL;
}

char* string_get(string_t* string)
{
	return string->data;
}
