#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pipes.h"

int internal_cd(int argc, char** argv, int in, int out, int return_value)
{
	if (argc != 1 && argc != 2)
	{
		if (fprintf(stderr, "PPshell: cd: too many arguments\n") < 0)
			return 2;

		return 1;
	}

	const char* new_pwd_value = NULL;

	if (argc == 1)
	{
		new_pwd_value = getenv("HOME");
		if (new_pwd_value == NULL)
		{
			fprintf(stderr, "PPshell: cd: HOME not set\n");
			return 1;
		}
	}
	else
	{
		const char* argument = argv[1];

		if (strcmp(argument, "-") == 0)
		{
			new_pwd_value = getenv("OLDPWD");
			if (new_pwd_value == NULL)
			{
				fprintf(stderr, "PPshell: cd: OLDPWD not set\n");
				return 1;
			}
			dprintf(out, "%s\n", new_pwd_value);
		}
		else
		{
			DIR* dir = opendir(argument);
			if (dir == NULL)
			{
				if (fprintf(stderr, "cd: %s is not a valid directory\n",
							argument) < 0)
					return 2;

				return 1;
			}
			closedir(dir);
			new_pwd_value = argument;
		}
	}

	const char* pwd = getenv("PWD");
	if (pwd != NULL && setenv("OLDPWD", pwd, 1) != 0)
	{
		if (fprintf(stderr, "setenv(OLDPWD) error\n") < 0)
			return 4;

		return 3;
	}

	if (setenv("PWD", new_pwd_value, 1) != 0)
	{
		if (fprintf(stderr, "setenv(PWD) error\n") < 0)
			return 6;

		return 5;
	}

	if (chdir(new_pwd_value) != 0)
	{
		if (fprintf(stderr, "chdir error\n") < 0)
			return 8;

		return 7;
	}

	return 0;

	(void)in;
	(void)return_value;
}
