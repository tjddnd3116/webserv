#include "clientSocket.hpp"

clientSocket::clientSocket(const configInfo& conf)
	:ASocket(conf)
{
	m_readBuffer.clear();
}

clientSocket::clientSocket(const ASocket& serverSock)
	:ASocket(serverSock.getConf())
{
	m_SocketFd = serverSock.getSocketFd();
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

int
clientSocket::readSock(void)
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
			request request(m_conf);

			m_method = request.readRequest(m_readBuffer);
			if (1)
			{
				std::cout << *m_method << std::endl;
				m_method->printBody();
			}
			m_readFinish = true;
			m_readBuffer.clear();
		}
	}
	return (readRet);
}

int
clientSocket::sendSock(void)
{
	int sendRet;
	response response(m_conf);

	std::cout << *m_method << std::endl;
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
clientSocket::getReadStatus(void) const
{
	return (m_readFinish);
}
