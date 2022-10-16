#include "cgi.hpp"
#include <cstddef>
#include <unistd.h>
#include <vector>

cgi::cgi()
{
}

cgi::~cgi()
{
}

void cgi::initCgi(const AMethod *method)
{
    cgi_path = "/Users/gson/Archive/webserv/html/php/php-cgi";
    std::string REDIRECT_STATUS = "REDIRECT_STATUS=200";
    std::string CONTENT_LENGTH = "CONTENT_LENGTH=" + method->getMethod();
    std::string SERVER_PROTOCOL = "QUERY_STRING=" + method->getHttpVersion();
    std::string REQUEST_METHOD = "REQUEST_METHOD=" + method->getMethod();
    std::string PATH_INFO = "PATH_INFO=/Users/gson/Archive/webserv/html/php";
    std::string QUERY_STRING = "QUERY_STRING=";
    std::string SCRIPT_NAME = "SCRIPT_NAME=php-cgi";
    if (method->getMethod() == "GET")
    {
        std::string uri = method->getUri();
        QUERY_STRING += uri.substr(uri.find("?") + 1);
    }
    else if (method->getMethod() == "POST")
    {
        std::string uri = method->getUri();
        QUERY_STRING += uri.substr(uri.find("?") + 1);
    }
    std::string SERVER_PORT;
    std::map<std::string, std::vector<std::string> >::const_iterator mapIt;
    mapIt = method->getRequestSet().find("Host");
    std::string port = mapIt->second[0];
    SERVER_PORT += port.substr(port.find(":") + 1);
    env.push_back(REDIRECT_STATUS);
    env.push_back(CONTENT_LENGTH);
    env.push_back(SERVER_PROTOCOL);
    env.push_back(REQUEST_METHOD);
    env.push_back(PATH_INFO);
    env.push_back(QUERY_STRING);
    env.push_back(SERVER_PORT);
    for (size_t i = 0; i < env.size(); i++)
	{
		cenv.push_back(const_cast<char*>(env[i].c_str()));
	}
    cenv.push_back(0);
}

void cgi::execCgi(const AMethod *method)
{
    int fd[2];
    int pid;
    char    buf[65000] = {0};
    std::string file_path;

    std::string uri = method->getUri();
    file_path = "/Users/gson/Archive/webserv/html/test/form.html";
    std::vector<char> file_p(file_path.begin(), file_path.end());
    file_p.push_back('\0');
    char **argv = new char*[2];
    argv[1] = NULL;
    char *file = &file_p[0];
    argv[0] = file;
    if (pipe(fd) == -1 || (pid = fork()) == -1)
    {
        exit(0);
    }
    if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        execve(cgi_path.c_str(), argv, &cenv[0]);
        std::cout << "fail\n" << std::endl;
    }
    else
    {
        wait(NULL);
        close(fd[1]);
        read(fd[0], buf, 65000);
        close(fd[0]);
        std::cout << "---- buf ----\n" << std::endl;
        std::cout << buf << std::endl;
        std::cout << "---- buf ----\n" << std::endl;
    }
}