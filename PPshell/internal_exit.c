#include <stdlib.h>
#include "return_value.h"
#include "pipes.h"

int internal_exit(int argc, char** argv, pipe_t p)
{
	exit(return_value);
}
