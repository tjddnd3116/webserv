#include "server.hpp"

server::server(const std::vector<configInfo> &conf, std::fstream& logFile)
	:m_conf(conf), m_logFile(logFile)
{
	m_kq = kqueue();
	if (m_kq < 0)
		throw (WsException("kqueue fail"));
}

server::~server()
{}

void
server::createServerSock(void)
{
	size_t serverSockSize;

	serverSockSize = m_conf.size();
	for (size_t serverSockIdx = 0; serverSockIdx < serverSockSize; serverSockIdx++)
	{
		serverSocket serverSock(m_conf[serverSockIdx]);

		serverSock.createSock();
		serverSock.initAddr();
		serverSock.bindSock();
		serverSock.listenSock();
		m_serverSock.insert(std::make_pair(serverSock.getSocketFd(), serverSock));
		addEvents(serverSock.getSocketFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	}
	//m_logFile << "all server socket created, server socket count : " << m_serverSock.size() << std::endl;
}

void
server::run(void)
{
	int newEvents;
	response::setStatusCode();

	while (1)
	{
		newEvents = waitEvent();
		communicateSock(newEvents);
	}
}

void
server::addEvents(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void* udata)
{
	struct kevent tempEvent;

	EV_SET(&tempEvent, ident, filter, flags, fflags, data, udata);
	m_changeList.push_back(tempEvent);
}

int
server::waitEvent(void)
{
	int newEvents;

	//m_logFile << "\n---waiting event---" << std::endl;
	newEvents = kevent(m_kq, &m_changeList[0], m_changeList.size(),
			m_eventList, EVENT_SIZE, NULL);
	//m_logFile << "new events count : " << newEvents << std::endl;
	if (newEvents == -1)
		throw (WsException("kevent fail"));
	m_changeList.clear();
	return (newEvents);
}

void
server::communicateSock(int newEvents)
{
	for (int i = 0; i < newEvents; i++)
	{
		struct kevent* curEvent;

		curEvent = &m_eventList[i];
		//m_logFile << "new event fd : " << curEvent->ident << std::endl;
		if (curEvent->filter == EVFILT_READ)
		{}
			//m_logFile << "[read] event occured" << std::endl;
		else if (curEvent->filter == -2)
		{}
			//m_logFile << "[write] event occured" << std::endl;
		if (curEvent->flags & EV_ERROR)
		{
			if (isServerSocket(curEvent->ident))
				throw (WsException("server socket fail"));
			if (isClientSocket(curEvent->ident))
				throw (WsException("client socket fail"));
		}
		else if (curEvent->filter == EVFILT_READ)
		{
			if (readEvent(curEvent))
					return ;
		}
		else if (curEvent->filter == EVFILT_WRITE)
			writeEvent(curEvent);
	}
}

int
server::readEvent(struct kevent* curEvent)
{
	if (isServerSocket(curEvent->ident))
	{
		clientSocket clientSock(m_serverSock.at(curEvent->ident));

		clientSock.createSock();
		m_clientSock.insert(std::make_pair(clientSock.getSocketFd(), clientSock));
		addEvents(clientSock.getSocketFd(), EVFILT_READ,
				EV_ADD | EV_ENABLE, 0, 0, NULL);
		//m_logFile << "client socket[" << clientSock.getSocketFd() << "] created, server read finish ";
		//m_logFile << "now client socket size : " << m_clientSock.size() << std::endl;
		return (1);
	}
	else if(isClientSocket(curEvent->ident))
	{
		int readRet;
		int msgSize;

		msgSize = curEvent->data + 1;
		clientSocket& clientSocket
			= (*m_clientSock.find(curEvent->ident)).second;
		readRet = clientSocket.readSock(m_logFile, msgSize);
		if (readRet <= 0)
		{
			if (readRet == 0)
				disconnectClientSocket(curEvent->ident);
			//m_logFile << "client read error" << std::endl;
			return (1);
		}
		if (clientSocket.getReadStatus())
			addEvents(clientSocket.getSocketFd(), EVFILT_WRITE,
				EV_ADD | EV_ENABLE, 0, 0, NULL);
		//m_logFile << "client read finish" << std::endl;
	}
	return (0);
}

void
server::writeEvent(struct kevent* curEvent)
{
	int sendRet;

	if (!isClientSocket(curEvent->ident))
		return;

	clientSocket& clientsocket
		= (*m_clientSock.find(curEvent->ident)).second;
	sendRet = clientsocket.sendSock(m_logFile);
	if (clientsocket.getWriteStatus())
	{
		//m_logFile << "INFO: send finished on socket = "
			// << clientsocket.getSocketFd() << std::endl;
		addEvents(curEvent->ident, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
	}
	else if (sendRet < 0)
	{}
		//m_logFile << "send(write) non blocking" << std::endl;
	else
	{}
		//m_logFile << "INFO: send = " << sendRet
			// << ", on socket = " << clientsocket.getSocketFd() << std::endl;
}

void
server::disconnectClientSocket(int fd)
{
	//m_logFile << "client disconnected : " << fd << std::endl;
	m_clientSock.at(fd).closeSock();
	m_clientSock.erase(fd);
}

bool
server::isServerSocket(int fd)
{
	if (m_serverSock.find(fd) != m_serverSock.end())
		return (true);
	return (false);
}

bool
server::isClientSocket(int fd)
{
	if (m_clientSock.find(fd) != m_clientSock.end())
		return (true);
	return (false);
}
