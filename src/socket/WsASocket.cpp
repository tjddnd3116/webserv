#include "WsASocket.hpp"

WsASocket::WsASocket(const WsConfigInfo& conf)
{
	m_SocketAddrSize = sizeof(m_SocketAddr);
	m_conf = conf;
}

WsASocket::~WsASocket()
{
}

WsASocket::WsASocket(const WsASocket& copy)
{
	*this = copy;
}

WsASocket&
WsASocket::operator=(const WsASocket& copy)
{
	m_conf = copy.m_conf;
	m_SocketAddr = copy.m_SocketAddr;
	m_SocketFd = copy.m_SocketFd;
	m_SocketAddrSize = copy.m_SocketAddrSize;
	return (*this);
}

void
WsASocket::closeSock()
{
	close(m_SocketFd);
}

int
WsASocket::getSocketFd(void) const
{
	return (m_SocketFd);
}

const WsConfigInfo&
WsASocket::getConf(void) const
{
	return (m_conf);
}
