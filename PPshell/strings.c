#include <stddef.h>
#include "strings.h"
#include <assert.h>

const int strings_default_capacity = 7;

string_t* string_malloc(size_t count)
{
	return malloc((count + 1) * sizeof(string_t));
}

void strings_construct(strings_t* strings)
{
	strings->buffer = string_malloc(strings_default_capacity);
	strings->length = 0;
	strings->capacity = strings_default_capacity;

	string_construct(strings->buffer);
}

void strings_construct_move(strings_t* strings, strings_t* other)
{
	*strings = *other;
	other->buffer = NULL;
	other->length = 0;
	other->capacity = 0;
}

string_t* strings_get_end(strings_t* strings)
{
	return strings->buffer + strings->length;
}

void strings_destroy(strings_t* strings)
{
	if (strings->buffer != NULL)
	{
		string_t* i;
		for (i = strings->buffer; i != strings_get_end(strings); ++i)
			string_destroy(i);
	}

	free(strings->buffer);
	strings->buffer = NULL;
}

static void strings_many_init_move(string_t* begin, const string_t* end, string_t* to)
{
	for (; begin != end; ++begin, ++to)
		string_construct_move(to, begin);
}

void strings_append_move(strings_t* strings, string_t* string)
{
	assert(strings->buffer != NULL);
	
	if (strings->length == strings->capacity)
	{
		strings->capacity *= 2;

		string_t* new_buffer = string_malloc(strings->capacity);

		strings_many_init_move(strings->buffer, strings_get_end(strings), new_buffer);

		free(strings->buffer);

		strings->buffer = new_buffer;
	}

	string_construct_move(strings_get_end(strings), string);

	++strings->length;

	string_construct(strings_get_end(strings));
}

char** strings_get(strings_t* strings)
{
	return (char**)strings->buffer;
}

size_t strings_get_count(strings_t* strings)
{
	return strings->length;
}

string_t* strings_get_front(strings_t* strings)
{
	if (strings->length != 0)
		return strings->buffer;
	else
		return NULL;
}
