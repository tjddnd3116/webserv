#include "ASocket.hpp"

ASocket::ASocket(const configInfo& conf)
{
	m_SocketAddrSize = sizeof(m_SocketAddr);
	m_conf = conf;
}

ASocket::~ASocket()
{
}

ASocket::ASocket(const ASocket& copy)
{
	*this = copy;
}

ASocket&
ASocket::operator=(const ASocket& copy)
{
	m_conf = copy.m_conf;
	m_SocketAddr = copy.m_SocketAddr;
	m_SocketFd = copy.m_SocketFd;
	m_SocketAddrSize = copy.m_SocketAddrSize;
	return (*this);
}

void
ASocket::closeSock()
{
	close(m_SocketFd);
}

int
ASocket::getSocketFd(void) const
{
	return (m_SocketFd);
}

const configInfo&
ASocket::getConf(void) const
{
	return (m_conf);
}
