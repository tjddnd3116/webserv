#ifndef WsServer_hpp
#define WsServer_hpp

#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <map>
#include <vector>
#include <iostream>

#include "WsInitializer.hpp"
#include "./parser/WsConfigInfo.hpp"
#include "./socket/WsClientSock.hpp"
#include "./socket/WsServerSock.hpp"

#define EVENT_SIZE 1

class WsServer
{
	private:
		// member variable
		std::vector<WsConfigInfo>	m_conf;
		std::map<int, WsServerSock>	m_serverSock;
		std::map<int, WsClientSock>	m_clientSock;
		std::vector<struct kevent>	m_changeList;
		struct kevent				m_eventList[EVENT_SIZE];
		int							m_kq;
		size_t						m_serverSize;

		void	addEvents(uintptr_t ident, int16_t filter, uint16_t flags,
				uint32_t fflags, intptr_t data, void* udata);
		bool	isServerSocket(int fd);
		bool	isClientSocket(int fd);

		int		kevent(void);
		void	keventSock(int newEvents);
		int		readEvent(struct kevent* curEvent);
		void	writeEvent(struct kevent* curEvent);
		void	disconnectClient(int fd);

	public:
		// Orthodox Canonical Form
		WsServer(const std::vector<WsConfigInfo>& conf);
		~WsServer();
		WsServer(const WsServer& copy);
		WsServer& operator=(const WsServer& copy);

		void		createServerSock(void);
		void 		run(void);
};
#endif //WsServer_hpp
