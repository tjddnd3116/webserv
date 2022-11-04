#include "clientSocket.hpp"

clientSocket::clientSocket(const configInfo& conf)
	:ASocket(conf), m_request(conf)
{
	m_readBuffer.clear();
	m_method = NULL;
	is_bodySection = false;
}

clientSocket::clientSocket(const ASocket& serverSock)
	:ASocket(serverSock.getConf()), m_request(serverSock.getConf())
{
	m_SocketFd = serverSock.getSocketFd();
	m_readBuffer.clear();
	m_method = NULL;
	is_bodySection = false;
}

clientSocket::~clientSocket()
{}

clientSocket::clientSocket(const clientSocket& copy)
	:ASocket(copy.m_conf), m_request(copy.m_conf)
{
	*this = copy;
}

clientSocket&
clientSocket::operator=(const clientSocket& copy)
{
	m_method = copy.m_method;
	m_conf = copy.m_conf;
	is_bodySection = copy.is_bodySection;
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
clientSocket::readSock(std::fstream& logFile, int msgSize)
{
	int		readRet;
	char*	buffer;
	int		requestStatus;

	logFile << "message size : " << msgSize << std::endl;
	buffer = new char[msgSize];
	m_readFinish = false;
	std::memset(buffer, 0, msgSize);
	readRet = read(m_SocketFd, buffer, msgSize - 1);
	// logFile << "-----origin request message-----" << std::endl;
	// logFile << buffer << std::endl;
	// logFile << "--------------------------------" << std::endl;
	if (readRet < 0)
		logFile << "non-blocking" << std::endl;
	else if (readRet == 0)
		logFile << "client socket close!" << std::endl;
	else
	{
		requestStatus = m_request.readRequest(buffer);
		if (requestStatus == READING)
			return (readRet);
		if (requestStatus == READ_FIN)
		{
			m_method = m_request.getMethod();
			m_request.setMethodToNull();
			m_method->logMethodInfo(logFile);
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

	// TODO
	// just test!
	if (m_method->getMethod() == "PUT" || m_method->getMethod() == "POST")
	{
		m_method = NULL;
		return (-1);
	}
	return (sendRet);
}

bool
clientSocket::getReadStatus(void) const
{
	return (m_readFinish);
}
