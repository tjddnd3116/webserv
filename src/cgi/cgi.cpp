#include "cgi.hpp"

#include <cstddef>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>

cgi::cgi()
{
    m_env.clear();
    m_envChar.clear();
    m_cgiPath = "";
    m_path = "";
    m_script = "";
    m_query = "";
	m_bodyFlag = false;
}

cgi::~cgi()
{}

void
cgi::initCgi(const AMethod *method)
{
	m_cgiPath = method->getLocation()->locCgiPass;
    std::string rootPath;
    rootPath = method->getConfig().getRootPath();
    std::string uri = method->getFilePath();

    if (uri.find("?") != std::string::npos)
    {
        m_path = uri.substr(0, uri.find("?"));
    }
    else
    {
        m_path = uri;
    }

    std::string CONTENT_LENGTH = "CONTENT_LENGTH=";
    std::string CONTENT_TYPE = "CONTENT_TYPE=";
    std::map<std::string, std::vector<std::string> >::const_iterator contentIt;
    contentIt = method->getRequestSet().find("Content-Type");
    if (contentIt != method->getRequestSet().end())
    {
        contentIt = method->getRequestSet().find("Content-Type");
        std::string type = contentIt->second[0];
        //type.erase(type.end() - 1, type.end()); // 캐리지리턴 삭제
        CONTENT_TYPE += type;
    }

	std::string	HTTP_X_SECRET_HEADER_FOR_TEST;
    contentIt = method->getRequestSet().find("X-Secret-Header-For-Test");
    if (contentIt != method->getRequestSet().end())
    {
        // contentIt = method->getRequestSet().find("x_secret_header_for_test");
        std::string type = contentIt->second[0];
		std::cout << "secret = " << type << std::endl;
        //type.erase(type.end() - 1, type.end()); // 캐리지리턴 삭제
    	HTTP_X_SECRET_HEADER_FOR_TEST += "HTTP_X_SECRET_HEADER_FOR_TEST=" + type;
    }
    if (method->getMethod() == "POST" && method->getBodySize() != -1)
    {
        CONTENT_LENGTH += std::to_string(method->getBodySize());
    }
    std::string REDIRECT_STATUS = "REDIRECT_STATUS=200"; // php-cgi direct exec
    std::string SERVER_PROTOCOL = "SERVER_PROTOCOL=HTTP/1.1"; // different GET POST
    std::string GATEWAY_INTERFACE = "GATEWAY_INTERFACE=CGI/1.1";
    std::string REQUEST_METHOD = "REQUEST_METHOD=" + method->getMethod();
    std::string REQUEST_URI = "REQUEST_URI=" + method->getUri();
    std::string PATH_INFO = "PATH_INFO=" + m_path;
    std::string PATH_TRANSLATED = "PATH_TRANSLATED=" + m_path;
    std::string SCRIPT_NAME = "SCRIPT_NAME=" + m_path;
    std::string SCRIPT_FILENAME = "SCRIPT_FILENAME=" + m_path;
    std::string QUERY_STRING = "QUERY_STRING=";
    if (method->getMethod() == "GET")
    {
        QUERY_STRING += method->getQueryString();
    }
    std::string SERVER_NAME = "SERVER_NAME=";
    std::string HTTP_HOST = "HTTP_HOST=";
    std::string SERVER_PORT = "SERVER_PORT=";
    std::map<std::string, std::vector<std::string> >::const_iterator mapIt;
    mapIt = method->getRequestSet().find("Host");
    std::string port = mapIt->second[0];
    if (port.find(":") != std::string::npos)
        SERVER_PORT += port.substr(port.find(":") + 1);
    else
        SERVER_PORT += "80";
    SERVER_NAME += port;
    HTTP_HOST += port.substr(0, port.find(":"));
    std::string REMOTE_ADDR = "REMOTE_ADDR=";
    std::string REMOTE_HOST = "REMOTE_HOST=" + port;
    std::string REMOTE_IDENT = "REMOTE_IDENT=";
    std::string SERVER_SOFTWARE = "SERVER_SOFTWARE=webserv/1.0";

    m_env.push_back(REDIRECT_STATUS);
    m_env.push_back(CONTENT_LENGTH);
    m_env.push_back(CONTENT_TYPE);
    m_env.push_back(SERVER_PROTOCOL);
    m_env.push_back(GATEWAY_INTERFACE);
    m_env.push_back(REQUEST_METHOD);
    m_env.push_back(REQUEST_URI);
    m_env.push_back(PATH_INFO);
    m_env.push_back(PATH_TRANSLATED);
    m_env.push_back(SCRIPT_NAME);
    m_env.push_back(SCRIPT_FILENAME);
    m_env.push_back(QUERY_STRING);
    m_env.push_back(SERVER_NAME);
    m_env.push_back(SERVER_PORT);
    m_env.push_back(HTTP_HOST);
    m_env.push_back(REMOTE_ADDR);
    m_env.push_back(REMOTE_HOST);
    m_env.push_back(REMOTE_IDENT);
    m_env.push_back(SERVER_SOFTWARE);
	m_env.push_back(HTTP_X_SECRET_HEADER_FOR_TEST);

    for (size_t i = 0; i < m_env.size(); i++)
	{
		m_envChar.push_back(const_cast<char*>(m_env[i].c_str()));
	}
    m_envChar.push_back(0);
}

// std::string
// cgi::execCgi(const std::string& readLine)
// {
//     int fd_A[2];
//     int fd_B[2];
//     int fd_in;
//     int fd_out;
//     pid_t pid;
//     std::string body;
//     std::string body_buffer;
//
//     fd_in = dup(STDIN_FILENO);
//     fd_out = dup(STDOUT_FILENO);
//     if (pipe(fd_A) == -1 || pipe(fd_B) == -1 || (pid = fork()) == -1)
//     {
//         exit(0);
//     }
//     fcntl(fd_B[0], F_SETFL, O_NONBLOCK);
//     fcntl(fd_B[1], F_SETFL, O_NONBLOCK);
//     if (pid == 0)
//     {
//         close(fd_A[READ]);
//         close(fd_B[WRITE]);
//         dup2(fd_B[READ], STDIN_FILENO);
//         dup2(fd_A[WRITE], STDOUT_FILENO);
//         close(fd_A[WRITE]);
//         execve(m_cgiPath.c_str(), NULL, &m_envChar[0]);
//     }
//     else
//     {
//         char    buf[1024] = {0};
//         body_buffer = readLine;
//
//         close(fd_A[WRITE]);
//         close(fd_B[READ]);
//         write(fd_B[WRITE], body_buffer.c_str(), body_buffer.length());
//         close(fd_B[WRITE]);
//
//         waitpid(-1, NULL, 0);
//         int ret = 1;
//         while (ret > 0)
//         {
//             memset(buf, 0, 1024);
//             ret = read(fd_A[READ], buf, 1024 - 1);
//             body += buf;
//         }
//         close(fd_A[READ]);
//     }
//     dup2(fd_in, STDIN_FILENO);
//     dup2(fd_out, STDOUT_FILENO);
//     close(fd_in);
//     close(fd_out);
//     return body;
// }

void
cgi::runCgi(void)
{
//	m_fd_in = dup(STDIN_FILENO);
//	m_fd_out = dup(STDOUT_FILENO);
	if (pipe(m_fromCgiToServer) == -1 || pipe(m_fromServerToCgi) == -1)
		exit(0);
	fcntl(m_fromServerToCgi[0], F_SETFL, O_NONBLOCK);
	fcntl(m_fromServerToCgi[1], F_SETFL, O_NONBLOCK);
	fcntl(m_fromCgiToServer[0], F_SETFL, O_NONBLOCK);
	fcntl(m_fromCgiToServer[1], F_SETFL, O_NONBLOCK);
	if ((m_pid = fork()) == -1)
		exit(1);
	if (m_pid == 0)
	{
		close(m_fromCgiToServer[READ]);
        close(m_fromServerToCgi[WRITE]);
        dup2(m_fromServerToCgi[READ], STDIN_FILENO);
        dup2(m_fromCgiToServer[WRITE], STDOUT_FILENO);
        close(m_fromCgiToServer[WRITE]);
		close(m_fromServerToCgi[READ]);
        execve(m_cgiPath.c_str(), NULL, &m_envChar[0]);
		throw std::runtime_error("ERROR: execve fail");
	}
	else
    {
		close(m_fromCgiToServer[WRITE]);
		close(m_fromServerToCgi[READ]);
	}
}

void	 cgi::closeCgi(int pipeEnd)
{
	if (pipeEnd == READ)
		close(m_fromCgiToServer[READ]);
	else if (pipeEnd == WRITE)
		close(m_fromServerToCgi[WRITE]);
//	dup2(m_fd_in, STDIN_FILENO);
//	dup2(m_fd_out, STDOUT_FILENO);
//	close(m_fd_in);
//	close(m_fd_out);
}

/*
std::string
cgi::execCgi(const std::string& readLine)
{
	std::string	body;
	std::string	body_buffer;
	char		buf[BUFFER_SIZE] = {0};

	body_buffer = readLine;
	write(m_fromServerToCgi[WRITE], body_buffer.c_str(), body_buffer.length());

	int ret = 1;
	do
	{
		while (ret > 0)
		{
			memset(buf, 0, BUFFER_SIZE);
			ret = read(m_fromCgiToServer[READ], buf, BUFFER_SIZE - 1);
			body += buf;
		}
	}
	while (body.size() == 0);
	// if (!m_bodyFlag && body.find("\r\n\r\n") != std::string::npos)
	// {
	//     m_bodyFlag = true;
	//     body = body.substr(body.find("\r\n\r\n") + 4);
	// }
	// close(m_fdA[READ]);
	return (body);
}
*/

std::string
cgi::readCgi()
{
	int			ret;
	std::string	body;
	char buf[BUFFER_SIZE] = {0};

    waitpid(-1, NULL, 0);
	do
	{
		do
		{
			memset(buf, 0, BUFFER_SIZE);
			ret = read(m_fromCgiToServer[READ], buf, BUFFER_SIZE - 1);
			body += buf;
		}
		while (ret > 0);
	}
	while (body.size() == 0);
	return (body);
}



ssize_t	cgi::writeCgi(const void* buf, size_t size)
{
	return write(m_fromServerToCgi[WRITE], buf, size);
	/*
	size_t		totalWriteBytes = 0;
	uintptr_t	ptr = reinterpret_cast<uintptr_t>(buf);

	while (size > 0)
	{
		void*	movedBuf = reinterpret_cast<void*>(ptr + totalWriteBytes);
		ssize_t	writeBytes = write(m_fromServerToCgi[WRITE], movedBuf, size);

		if (writeBytes == -1)
			continue;
		totalWriteBytes += writeBytes;
		size -= writeBytes;
	}
	return totalWriteBytes;
	*/
}

std::string
cgi::readCloseCgi(void)
{
	int			ret = 1;
	std::string	body;
	char buf[BUFFER_SIZE] = {0};

	while (ret != 0)
	{
		memset(buf, 0, BUFFER_SIZE);
		ret = read(m_fromCgiToServer[READ], buf, BUFFER_SIZE - 1);
		body += buf;
	}
	return (body);
}
