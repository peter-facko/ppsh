#include <stdlib.h>

#include "pipes.h"

int internal_exit(int argc, char** argv, int in, int out, int return_value)
{
	exit(return_value);

	(void)argc;
	(void)argv;
	(void)in;
	(void)out;
}
