#pragma once
#include <stdbool.h>

/** Wrapper for a null-terminated string of characters. */
typedef struct string
{
	char* data;
} string_t;

/** Constructs an invalid string_t. */
void string_construct(string_t* string);
void string_construct_move(string_t* string, string_t* new_string);
void string_assign_move(string_t* string, string_t* new_string);
void string_destroy(string_t* string);
bool string_is_valid(const string_t* string);

/** Copies characters from null-terminated 'data'. */
int string_construct_cstring_copy(string_t* string, const char* data);

/** Obtains a pointer to a null terminated array of characters in 'string'.
 *
 * @retval NULL - 'string' is invalid.
 */
char* string_get(string_t* string);
