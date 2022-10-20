#include <iostream>

int main(int argc, char** argv, char *envp[])
{
    while (envp)
    {
        std::cout << *envp << std::endl;
        (envp)++;
    }
    while (argv)
    {
        std::cout << *argv << std::endl;
        (argv)++;
    }
    return 0;
}