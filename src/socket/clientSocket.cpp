#include "clientSocket.hpp"
#include <string>

clientSocket::clientSocket(const configInfo& conf)
	:ASocket(conf)
{
	m_readBuffer.clear();
	m_method = NULL;
	is_bodySection = 0;
}

clientSocket::clientSocket(const ASocket& serverSock)
	:ASocket(serverSock.getConf())
{
	m_SocketFd = serverSock.getSocketFd();
	m_readBuffer.clear();
	m_method = NULL;
	is_bodySection = 0;
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
		if (readRet == BUF_SIZE)
			return (readRet);
		if (m_readBuffer.rfind("\r\n\r\n") == std::string::npos)
		{
			return readRet;
		}
		else if (is_bodySection == false && \
			m_readBuffer.rfind("\r\n\r\n") != std::string::npos)
		{
			std::cout << "Found carrage return!" << std::endl;
			request request(m_conf);
			m_method = request.readRequest(m_readBuffer);
			if (m_method->getMethod() == "POST")
			{
				
				//postMethod* 	tempPost = dynamic_cast<postMethod*>(m_method);
				//tempPost->loadBody(m_readBuffer);
				std::cout << "m_body is : " << m_method->getBody() << std::endl;
				std::map<std::string, std::vector<std::string> >::const_iterator transferIt;
				transferIt = m_method->getRequestSet().find("Transfer-Encoding");
				std::string type;
				if (transferIt != m_method->getRequestSet().end())
				{
				    type = transferIt->second[0];
				}
				if (type == "chunked" && type.find("0\n") == std::string::npos)
				{
					is_bodySection = true;
					return readRet;
				}
			}
			if (m_method->getMethod() != "POST")
			{
				is_bodySection = false;
				std::cout << *m_method << std::endl;
				m_method->printBody();
			}
			m_readFinish = true;
			m_readBuffer.clear();
		}
		else if (is_bodySection == true && \
			m_readBuffer.rfind("\r\n\r\n") != std::string::npos)
		{
			std::cout << *m_method << std::endl;
			m_method->printBody();
			m_readFinish = true;
			is_bodySection = false;
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

	response.makeResponse(m_method);
	if (0)
	{
		std::cout << BLUE << "-----------response----------------" << std::endl;
		std::cout << response().c_str() << std::endl;
		std::cout << "-------------------------------" << RESET << std::endl;
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
