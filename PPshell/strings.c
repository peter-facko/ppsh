#include "strings.h"

#include <assert.h>
#include <stddef.h>

#include "error_handling.h"

const int strings_default_capacity = 7;

string_t* strings_get_begin(strings_t* strings)
{
	assert(strings != NULL);
	assert(strings_is_valid(strings));

	return strings->buffer;
}

static string_t* strings_get_end(strings_t* strings)
{
	assert(strings != NULL);
	assert(strings_is_valid(strings));

	return strings_get_begin(strings) + strings->length;
}

string_t* string_malloc(size_t count)
{
	return (string_t*)malloc((count + 1) * sizeof(string_t));
}

int strings_construct(strings_t* strings)
{
	ERROR_CHECK_PTR_NEG_ONE(strings->buffer =
								string_malloc(strings_default_capacity));

	strings->length = 0;
	strings->capacity = strings_default_capacity;

	string_construct(strings->buffer);

	return 0;
}

void strings_construct_move(strings_t* strings, strings_t* other)
{
	*strings = *other;
	other->buffer = NULL;
	other->length = 0;
	other->capacity = 0;
}

void strings_destroy(strings_t* strings)
{
	assert(strings != NULL);

	if (strings_is_valid(strings))
	{
		for (string_t* i = strings_get_begin(strings);
			 i != strings_get_end(strings); ++i)
			string_destroy(i);
	}

	free(strings->buffer);
	strings->buffer = NULL;
}

bool strings_is_valid(const strings_t* strings)
{
	assert(strings != NULL);

	return strings->buffer != NULL;
}

static void strings_many_construct_move(string_t* begin, const string_t* end,
										string_t* to)
{
	assert(begin != NULL);
	assert(end != NULL);
	assert(to != NULL);

	for (; begin != end; ++begin, ++to)
		string_construct_move(to, begin);
}

int strings_append_move(strings_t* strings, string_t* string)
{
	assert(strings != NULL);
	assert(string != NULL);
	assert(strings_is_valid(strings));
	assert(string_is_valid(string));

	if (strings->length == strings->capacity)
	{
		size_t new_capacity = strings->capacity * 2 + 1;

		string_t* new_buffer;
		ERROR_CHECK_PTR_NEG_ONE(new_buffer = string_malloc(new_capacity));

		strings->capacity = new_capacity;

		strings_many_construct_move(strings_get_begin(strings),
									strings_get_end(strings), new_buffer);

		free(strings->buffer);

		strings->buffer = new_buffer;
	}

	string_construct_move(strings_get_end(strings), string);

	++strings->length;

	string_construct(strings_get_end(strings));

	return 0;
}

char** strings_get(strings_t* strings)
{
	assert(strings != NULL);

	return (char**)strings_get_begin(strings);
}

size_t strings_get_count(const strings_t* strings)
{
	assert(strings != NULL);

	return strings->length;
}
