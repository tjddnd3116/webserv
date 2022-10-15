#include "WsClientSock.hpp"
#include "WsASocket.hpp"
#include "WsResponse.hpp"
#include <sys/fcntl.h>
#include <sys/socket.h>

WsClientSock::WsClientSock(const WsConfigInfo& conf)
	:WsASocket(conf)
{
	m_readBuffer.clear();
}

WsClientSock::WsClientSock(const WsASocket& serverSock)
	:WsASocket(serverSock.getConf())
{
	m_SocketFd = serverSock.getSocketFd();
}

WsClientSock::~WsClientSock()
{}

WsClientSock::WsClientSock(const WsClientSock& copy)
	:WsASocket(copy.m_conf)
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
			std::cout << "read size : " << readRet << std::endl;
			WsRequest request;
			m_method = request.readRequest(m_readBuffer);
			if (1)
				m_method->printInfo();
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
	WsResponse response(m_conf);

	response.makeResponse(m_method);
	if (0)
	{
		std::cout << BLUE << "-----------response----------------" << std::endl;
		std::cout << response().c_str() << std::endl;
		std::cout << "-------------------------------" << RESET << std::endl;
	}
	sendRet = write(m_SocketFd, response().c_str(), response.getBufSize());
	return (sendRet);
}

bool
WsClientSock::getReadStatus(void) const
{
	return (m_readFinish);
}
