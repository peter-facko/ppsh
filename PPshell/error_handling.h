#pragma once
#include <stddef.h>

/**Used for simple error return code checking.
 *
 * Uses return statement.
 */
#define ERROR_CHECK(expression, return_expr)                                   \
	do                                                                         \
	{                                                                          \
		if (expression)                                                        \
			return return_expr;                                                \
	} while (0)

/** Checks the value of 'expression' and returns 'return_expr' if it does not
 * equal 0. */
#define ERROR_CHECK_INT(expression, return_expr)                               \
	ERROR_CHECK((expression) != 0, return_expr)

/** Checks the value of 'expression' and returns 'return_expr' if it does not
 * equal 0. */
#define ERROR_CHECK_PTR(expression, return_expr)                               \
	ERROR_CHECK((expression) == NULL, return_expr)

/** Checks the value of 'expression' and returns -1 if it does not equal 0. */
#define ERROR_CHECK_INT_NEG_ONE(expression) ERROR_CHECK_INT(expression, -1)
/** Checks the value of 'expression' and returns -1 if it equals NULL. */
#define ERROR_CHECK_PTR_NEG_ONE(expression) ERROR_CHECK_PTR(expression, -1)
