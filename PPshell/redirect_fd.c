#include <unistd.h>

#include "redirect_fd.h"

void redirect_fd(int to_change, int new_fd)
{
	if (to_change != new_fd)
	{
		dup2(new_fd, to_change);
		close(new_fd);
	}
}
