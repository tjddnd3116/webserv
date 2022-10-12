#include "WsClientSock.hpp"
#include "WsASocket.hpp"
#include "WsResponse.hpp"
#include <sys/fcntl.h>
#include <sys/socket.h>

WsClientSock::WsClientSock(const WsConfigInfo& conf)
	:WsASocket(conf), m_response(conf)
{}

WsClientSock::WsClientSock(const WsASocket& serverSock)
	:WsASocket(serverSock.getConf()), m_response(serverSock.getConf())
{
	m_SocketFd = serverSock.getSocketFd();
}

WsClientSock::~WsClientSock()
{}

WsClientSock::WsClientSock(const WsClientSock& copy)
	:WsASocket(copy.m_conf), m_response(copy.m_response)
{
	*this = copy;
}

WsClientSock&
WsClientSock::operator=(const WsClientSock& copy)
{
	m_conf = copy.m_conf;
	m_SocketAddr = copy.m_SocketAddr;
	m_SocketAddrSize = copy.m_SocketAddrSize;
	m_SocketFd = copy.m_SocketFd;
	m_request = copy.m_request;
	m_response = copy.m_response;
	return (*this);
}


void
WsClientSock::createSock(void)
{
	int opts;

	m_SocketFd = accept(m_SocketFd, (struct sockaddr*)&m_SocketAddr,
			&m_SocketAddrSize);
	opts = fcntl(m_SocketFd, F_SETFL);
	opts = (opts | O_NONBLOCK);
	fcntl(m_SocketFd, F_SETFL, opts);
	if (m_SocketFd < 0)
		throw WsException("create(accept) socket fail");
	// std::cout << "created(accept) client socket : " << m_SocketFd << std::endl;
}

int
WsClientSock::readSock(void)
{
	int readRet;
	char buffer[BUF_SIZE];

	m_readFinish = false;
	std::memset(buffer, 0, sizeof(buffer));
	readRet = read(m_SocketFd, buffer, BUF_SIZE);
	if (readRet < 0)
		std::cout << "non-blocking" << std::endl;
	else if (readRet == 0)
		std::cout << "client socket close!" << std::endl;
	else
	{
		m_readBuffer += buffer;
		if (readRet < BUF_SIZE)
		{
			m_request.readRequest(m_readBuffer);
			if (1)
				m_request.printRequest();
			m_readFinish = true;
			m_readBuffer.clear();
		}
	}
	return (readRet);
}

int
WsClientSock::sendSock(void)
{
	int sendRet;
	// char buffer[BUF_SIZE];

	// if (m_writeFinish)
	//     return (1);
	// if (m_request.getMethod() == NULL)
	// {
	//     read(m_SocketFd, buffer, BUF_SIZE);
	//     std::cout << "strange event" << std::endl;
	//     std::cout << "$" << buffer << "$" << std::endl;
	//     write(m_SocketFd, "hello?", 7);
	//     return (-1);
	// }
	// if (!m_writeFinish)
	//
	m_response.clearBuffer();
	m_response.makeResponse(m_request.getMethod());
	if (1)
	{
		std::cout << BLUE << "-----------response----------------" << std::endl;
		std::cout << m_response().c_str() << std::endl;
		std::cout << "-------------------------------" << RESET << std::endl;
	}
	sendRet = write(m_SocketFd, m_response().c_str(), m_response.getBufSize());
	m_writeFinish = true;
	return (sendRet);
}

bool
WsClientSock::getReadStatus(void) const
{
	return (m_readFinish);
}

bool
WsClientSock::getwriteStatus(void) const
{
	return (m_writeFinish);
}
