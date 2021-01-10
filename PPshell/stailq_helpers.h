#pragma once

/** Creates helper static function defintions for a structure that uses STAILQ.
 *
 * Creates:
 * 		list_type##_get_head_const - gets the head of STAILQ
 * 		list_type##_get_head - non-const version
 * 		list_type##_pop_and_destroy - pops the first elements and destroys it
 *
 * The 'element_type' needs to implement `element_type##_destroy` special
 * function.
 */
#define CREATE_STAILQ_HELPERS(list_type, head_type, head_name, element_type,   \
							  link_name)                                       \
	static const struct head_type* list_type##_get_head_const(                 \
		const struct list_type* l)                                             \
	{                                                                          \
		assert(l != NULL);                                                     \
                                                                               \
		return &l->head_name;                                                  \
	}                                                                          \
	static struct head_type* list_type##_get_head(struct list_type* l)         \
	{                                                                          \
		assert(l != NULL);                                                     \
                                                                               \
		return (struct head_type*)list_type##_get_head_const(l);               \
	}                                                                          \
	static struct element_type* list_type##_pop_and_destroy(                   \
		struct list_type* l)                                                   \
	{                                                                          \
		assert(l != NULL);                                                     \
                                                                               \
		struct element_type* e = STAILQ_FIRST(list_type##_get_head(l));        \
		element_type##_destroy(e);                                             \
		STAILQ_REMOVE(list_type##_get_head(l), e, element_type, link);         \
		return e;                                                              \
	}
