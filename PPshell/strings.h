#pragma once
#include <stdlib.h>

#include "string.h"

/** String container. */
typedef struct strings
{
	string_t* buffer;
	size_t length;
	size_t capacity;
} strings_t;

/** Constructs a valid empty strings_t. */
int strings_construct(strings_t* strings);
/** Special function. */
void strings_construct_move(strings_t* strings, strings_t* other);
/** Special function. */
void strings_destroy(strings_t* strings);
/** Special function. */
bool strings_is_valid(const strings_t* strings);

/** Appends a new strings_t at the end of 'strings' with moved content from
 * 'string'. */
int strings_append_move(strings_t* strings, string_t* string);

/** Gets the address of the first element of 'strings'.
 *
 * Call on an empty 'strings' is valid, but the return value is undefined.
 */
string_t* strings_get_begin(strings_t* strings);
/** Gets the 'strings' as a null-terminated array of cstrings. */
char** strings_get(strings_t* strings);
/** Gets the number of elements in 'strings'. */
size_t strings_get_count(const strings_t* strings);
