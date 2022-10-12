#include "WsServerSock.hpp"
#include "WsASocket.hpp"

WsServerSock::WsServerSock(const WsConfigInfo& conf)
	:WsASocket(conf)
{}

WsServerSock::~WsServerSock()
{}

void WsServerSock::createSock(void)
{
	int option;

	option = 1;
	m_SocketFd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(m_SocketFd, F_SETFL, O_NONBLOCK);
	if (m_SocketFd == -1)
		throw WsException("create socket fail");
	setsockopt(m_SocketFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
}

void WsServerSock::initAddr()
{
	std::memset(&m_SocketAddr, 0, sizeof(m_SocketAddr));
	m_SocketAddr.sin_family = AF_INET;
	m_SocketAddr.sin_port = htons(m_conf.getListenPort()[0]);
	m_SocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

void WsServerSock::bindSock(void)
{
	int bindRet;

	bindRet = bind(m_SocketFd, (const struct sockaddr*)&m_SocketAddr, sizeof(m_SocketAddr));
	if (bindRet == -1)
		throw WsException("bind socket fail");
}

void WsServerSock::listenSock(void)
{
	int listenRet;

	listenRet = listen(m_SocketFd, 5);
	if (listenRet == -1)
		throw WsException("listen socket fail");
}


