#include <pipes.hpp>

#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <unistd.h>

namespace ppsh::builtins
{
int cd(int argc, char** argv, int, int out, int)
{
    if (argc != 1 && argc != 2)
    {
        std::cerr << "ppsh: cd: too many arguments\n";

        return 1;
    }

    const char* new_pwd_value = NULL;

    if (argc == 1)
    {
        new_pwd_value = std::getenv("HOME");
        if (new_pwd_value == NULL)
        {
            std::cerr << "ppsh: cd: HOME not set\n";
            return 1;
        }
    }
    else
    {
        std::string_view argument = argv[1];

        if (argument == "-")
        {
            new_pwd_value = std::getenv("OLDPWD");
            if (new_pwd_value == NULL)
            {
                std::cerr << "ppsh: cd: OLDPWD not set\n";
                return 1;
            }
            dprintf(out, "%s\n", new_pwd_value);
        }
        else
        {
            DIR* dir = opendir(argument.data());
            if (dir == NULL)
            {
                std::cerr << "cd: " << argument
                          << " is not a valid directory\n";

                return 1;
            }
            closedir(dir);
            new_pwd_value = argument.data();
        }
    }

    const char* pwd = std::getenv("PWD");
    if (pwd != NULL && setenv("OLDPWD", pwd, 1) != 0)
    {
        std::cerr << "setenv(OLDPWD) error\n";

        return 3;
    }

    if (setenv("PWD", new_pwd_value, 1) != 0)
    {
        std::cerr << "setenv(PWD) error\n";

        return 5;
    }

    if (chdir(new_pwd_value) != 0)
    {
        std::cerr << "chdir error\n";

        return 7;
    }

    return 0;
}
}
