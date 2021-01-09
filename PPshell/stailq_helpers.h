#pragma once

/** Creates a static function defintion which pops the first element from a
 * structure using STAILQ.
 *
 * The 'list_type' needs to implement `list_type##_get_head` function, which
 * returns the head of the STAILQ.
 *
 * The 'element_type' needs to implement `element_type##_destroy` special
 * function.
 *
 * The popped element is also destroyed.
 */
#define CREATE_STAILQ_POP_AND_DESTROY(list_type, element_type, link_name)      \
	static struct element_type* list_type##_pop_and_destroy(                   \
		struct list_type* l)                                                   \
	{                                                                          \
		struct element_type* e = STAILQ_FIRST(list_type##_get_head(l));        \
		element_type##_destroy(e);                                             \
		STAILQ_REMOVE(list_type##_get_head(l), e, element_type, link);         \
		return e;                                                              \
	}
