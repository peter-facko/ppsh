# API Semantics

This document serves as a "documentation template" to avoid documenting basically the same functions many times.

This documents only the user-defined structures in the PPshell project.

# States

Each object can be in one of three states:
* `undefined`
* `invalid`
* `valid`

`valid` and `invalid` are both `defined` states.

# Lifetime

Before any call to a [contructor](#Constructors), an object is in the `undefined` state.

No function can change an object's state from a `defined` state to `undefined`.

A simple correct lifetime of an object of an example structure `example_t` looks like:

```
example_t object;
example_construct(&object);
//...
example_do_something(&object);
//...
example_destroy(&object);
```

# Special functions

These functions are used to handle the lifetime of an object.

## Move

Pseudo function.

Parameters:
1. object
2. `{struct_name}*` moved-from object

Return type:
`void`

Preconditions:
* The object and the moved-from object are in `defined` states.

Postconditions:
* The object is in the same state as the moved-from object was before the call.
* The moved-from object is in the `invalid` state.

The object semantically "steals" resources from the moved-from object.

## Constructors

Naming format:
`{struct_name}_construct_{description}`.

Parameters:
1. `{struct_name}*` object (to construct)
2. any other parameters

Return type:
any

Preconditions:
* The object is not in the `valid` state.

Postconditions:
* The object is in a `defined` state.

Constructing an object means initializing it's internal structure.

Constructors must specify whether they move the object into the `valid` or `invalid` state.

Generally, constructors may fail.
If a constructor may fail, it must signalize failure with a return code.

### Move Constructors

Naming format:
`{struct_name}_construct_move`.

Parameters:
1. `{struct_name}*` object (to construct)
2. `{struct_name}*` moved-from object

Return type:
`void`.

Preconditions:
same as [constructors](#Constructors) plus [move](#Move).

Postconditions:
same as [constructors](#Constructors) plus [move](#Move).

[Moves](#Move) the moved-from object into object.

Cannot fail.

## Destructor

Naming format:
`{struct_name}_destroy`.

Parameters:
1. object to destroy

Return type:
`void`

Preconditions:
* The object is in a `defined` state.

Postconditions:
* The object is in the `invalid` state.

Destroying an object means disposing of its resources and moving it into the `invalid` state.

## Assigners

Naming format:
`{struct_name}_assign_{desription}`.

Parameters:
1. `{struct_name}*` object (to assign)
1. `{struct_name}*` other object (to assign from)

Return type:
any

Preconditions:
* The object and the other object are in `defined` states.

Postconditions:
* The object is in the same state as the moved-from object was before the call.
* The object is semantically equivalent to the other object before the call.

### Move assigners

Naming format:
`{struct_name}_assign_move`.

Parameters:
1. `{struct_name}*` object (to construct)
2. `{struct_name}*` moved-from object

Return type:
`void`

Semantically identical to:
```
void example_assign_move(example_t* object, example_t* moved_from_object)
{
	example_destroy(object);
	example_construct_move(object, moved_from_object);
}
```

## Valid Check

Naming format:
`{struct_name}_is_valid`.

Parameters:
1. `const {struct_name}*` object to check

Return type:
`bool`

Preconditions:
* The object is in a `defined` state.

Postconditions:
* The object is unchanged.

Checks whether the object is in the `valid` state.

# Ordinary functions

Any other functions not related to object lifetime.

Naming format:
`{struct_name}_{description}`.

Parameters:
1. object to execute the function on
2. any other parameters

Return type:
any

Preconditions:
* The object is in a `defined` state.

Postconditions:
* The object is in a `defined` state.
