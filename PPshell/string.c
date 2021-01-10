#include "string.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "error_handling.h"

void string_construct(string_t* string)
{
	assert(string != NULL);

	string->data = NULL;
}
void string_construct_move(string_t* string, string_t* new_string)
{
	assert(string != NULL);
	assert(new_string != NULL);

	string->data = new_string->data;
	new_string->data = NULL;
}
int string_construct_cstring_copy(string_t* string, const char* data)
{
	assert(string != NULL);
	assert(data != NULL);

	ERROR_CHECK_PTR_NEG_ONE(string->data = strdup(data));

	return 0;
}
void string_assign_move(string_t* string, string_t* new_string)
{
	assert(string != NULL);
	assert(new_string != NULL);

	if (string != new_string)
	{
		string_destroy(string);
		string_construct_move(string, new_string);
	}
}

void string_destroy(string_t* string)
{
	assert(string != NULL);

	free(string->data);
	string->data = NULL;
}

bool string_is_valid(const string_t* string)
{
	assert(string != NULL);

	return string->data != NULL;
}

char* string_get(string_t* string)
{
	assert(string != NULL);

	return string->data;
}
