#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv, char *env[])
{
    char *len_ = getenv("CONTENT_LENGTH");
    long len = strtol(len_, NULL, 10);
    char *postdata = new char[len];
    if (!postdata) { /* handle error or */ exit(EXIT_FAILURE); }
    fgets(postdata, len + 1, stdin);
    /* work with postdata */
    std::cout << postdata << std::endl;
    delete [] postdata;
}