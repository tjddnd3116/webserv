#include "clientSocket.hpp"

clientSocket::clientSocket(const ASocket& serverSock)
	:ASocket(serverSock.getConf()), m_request(serverSock.getConf())
{
	m_SocketFd = serverSock.getSocketFd();
	m_responsePtr = NULL;
	m_method = NULL;
	m_sentSize = 0;
	m_readBuffer.clear();
	m_readFinish = false;
	m_writeFinish = false;
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
	m_SocketAddr = copy.m_SocketAddr;
	m_SocketAddrSize = copy.m_SocketAddrSize;
	m_conf = copy.m_conf;
	m_SocketFd = copy.m_SocketFd;
	m_responsePtr = NULL;
	m_method = copy.m_method;
	m_sentSize = 0;
	m_readFinish = false;
	m_writeFinish = false;
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

	m_readFinish = false;
	logFile << "message size : " << msgSize << std::endl;
	buffer = new char[msgSize];
	std::memset(buffer, 0, msgSize);
	readRet = read(m_SocketFd, buffer, msgSize - 1);
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
			m_method->doMethodWork();
		}
	}
	return (readRet);
}

int
clientSocket::sendSock(std::fstream& logFile)
{
	int sendRet;

	if (m_sentSize == 0)
	{
		m_writeFinish = false;
		m_responsePtr = new response(m_conf);
		m_responsePtr->makeResponse(m_method);
		if (1)
		{
			logFile << BLUE << "-----------response----------------" << std::endl;
			logFile << (*m_responsePtr)().c_str() << std::endl;
			logFile << "-------------------------------" << RESET << std::endl;
		}
	}
	sendRet = write(m_SocketFd, (*m_responsePtr)().c_str() + m_sentSize, (*m_responsePtr).getBufSize() - m_sentSize);
	m_sentSize += sendRet;
	if (m_sentSize == (*m_responsePtr).getBufSize())
	{
		m_sentSize = 0;
		m_writeFinish = true;
		if (m_method->getMethod() == "PUT" || m_method->getMethod() == "POST")
		{
			delete m_responsePtr;
			return (-1);
		}
		delete m_responsePtr;
		return 0;
	}
	return (sendRet);
}

bool
clientSocket::getReadStatus(void) const
{
	return (m_readFinish);
}

bool  clientSocket::getWriteStatus(void) const
{
	return (m_writeFinish);
}
