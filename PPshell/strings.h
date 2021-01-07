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

/** Constructs a strings_t in 'strings' by moving the content of 'other'. */
void strings_construct_move(strings_t* strings, strings_t* other);

/** Appends a new strings_t at the end of 'strings' with moved content from
 * 'string'. */
int strings_append_move(strings_t* strings, string_t* string);

/** Destroys 'strings'. */
void strings_destroy(strings_t* strings);

string_t* strings_get_end(strings_t* strings);
char** strings_get(strings_t* strings);
size_t strings_get_count(strings_t* strings);

/**
 * @return Pointer to the first element, NULL if empty.
 */
string_t* strings_get_front(strings_t* strings);
