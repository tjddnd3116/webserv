#include "clientSocket.hpp"
#include <string>

clientSocket::clientSocket(const configInfo& conf)
	:ASocket(conf)
{
	m_readBuffer.clear();
	m_method = NULL;
}

clientSocket::clientSocket(const ASocket& serverSock)
	:ASocket(serverSock.getConf())
{
	m_SocketFd = serverSock.getSocketFd();
	m_readBuffer.clear();
	m_method = NULL;
}

clientSocket::~clientSocket()
{}

clientSocket::clientSocket(const clientSocket& copy)
	:ASocket(copy.m_conf)
{
	*this = copy;
}

clientSocket&
clientSocket::operator=(const clientSocket& copy)
{
	m_method = copy.m_method;
	m_conf = copy.m_conf;
	m_SocketAddr = copy.m_SocketAddr;
	m_SocketAddrSize = copy.m_SocketAddrSize;
	m_SocketFd = copy.m_SocketFd;
	return (*this);
}

void
clientSocket::createSock(void)
{
	int opts;

	m_SocketFd = accept(m_SocketFd, (struct sockaddr*)&m_SocketAddr,
			&m_SocketAddrSize);
	opts = fcntl(m_SocketFd, F_SETFL);
	opts = (opts | O_NONBLOCK);
	fcntl(m_SocketFd, F_SETFL, opts);
	if (m_SocketFd < 0)
		throw WsException("create(accept) socket fail");
}

// int
// clientSocket::readSock(std::fstream& logFile)
// {
//     int readRet;
//     char buffer[BUF_SIZE];
//
//     m_readFinish = false;
//     std::memset(buffer, 0, sizeof(buffer));
//     readRet = read(m_SocketFd, buffer, BUF_SIZE);
//
//     if (readRet < 0)
//         logFile << "non-blocking" << std::endl;
//     else if (readRet == 0)
//         logFile << "client socket close!" << std::endl;
//     else
//     {
//         m_readBuffer += buffer;
//         if (readRet == BUF_SIZE)
//             return (readRet);
//         if (m_method != NULL && m_method->getMethod() == "POST")
//         {
//             postMethod* 	tempPost = dynamic_cast<postMethod*>(m_method);
//
//             tempPost->loadBody(m_readBuffer);
//
//             unsigned int	contentLen = std::stoi((tempPost->getRequestSet()).at("Content-Length")[0]);
//             if (tempPost->getBody().size() < contentLen)
//                 return readRet;
//             else
//             {
//                 m_readFinish = true;
//                 m_readBuffer.clear();
//                 tempPost->printBody();
//             }
//         }
//         else if (m_readBuffer.rfind("\r\n\r\n") != std::string::npos)
//         {
//             request request(m_conf);
//             m_method = request.readRequest(m_readBuffer);
//             if (1)
//             {
//                 logFile << *m_method << std::endl;
//                 m_method->printBody();
//             }
//             if (m_method->getMethod() == "POST")
//             {
//                 postMethod* 	tempPost = dynamic_cast<postMethod*>(m_method);
//                 unsigned int	contentLen = std::stoi((tempPost->getRequestSet()).at("Content-Length")[0]);
//                 if (tempPost->getBody().size() < contentLen)
//                     return readRet;
//             }
//             m_readFinish = true;
//             m_readBuffer.clear();
//         }
//     }
//     return (readRet);
// }

int
clientSocket::readSock(std::fstream& logFile)
{
	int readRet;
	char buffer[BUF_SIZE];

	m_readFinish = false;
	std::memset(buffer, 0, sizeof(buffer));
	readRet = read(m_SocketFd, buffer, BUF_SIZE);
	if (readRet < 0)
		logFile << "non-blocking" << std::endl;
	else if (readRet == 0)
		logFile << "client socket close!" << std::endl;
	else
	{
		m_readBuffer += buffer;
		if (readRet < BUF_SIZE)
		{
			logFile << "read size : " << readRet << std::endl;
			request request(m_conf);

			m_method = request.readRequest(m_readBuffer);
			if (1)
			{
				logFile << *m_method << std::endl;
				m_method->printBody();
			}
			m_readFinish = true;
			m_readBuffer.clear();
		}
	}
	return (readRet);
}

int
clientSocket::sendSock(std::fstream& logFile)
{
	int sendRet;
	response response(m_conf);

	response.makeResponse(m_method);
	if (1)
	{
		logFile << BLUE << "-----------response----------------" << std::endl;
		logFile << response().c_str() << std::endl;
		logFile << "-------------------------------" << RESET << std::endl;
	}
	sendRet = write(m_SocketFd, response().c_str(), response.getBufSize());
	m_method = NULL;
	return (sendRet);
}

bool
clientSocket::getReadStatus(void) const
{
	return (m_readFinish);
}
