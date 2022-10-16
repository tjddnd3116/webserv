#ifndef server_hpp
#define server_hpp

#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <map>
#include <vector>
#include <iostream>

#include "./parser/configInfo.hpp"
#include "./socket/WsClientSock.hpp"
#include "./socket/WsServerSock.hpp"
#include "./socket/WsResponse.hpp"

#define EVENT_SIZE 8

class server
{
	private:
		// member variable
		std::vector<configInfo>		m_conf;
		std::map<int, WsServerSock>	m_serverSock;
		std::map<int, clientSock>	m_clientSock;
		std::vector<struct kevent>	m_changeList;
		struct kevent				m_eventList[EVENT_SIZE];
		int							m_kq;
		size_t						m_serverSize;

		void	addEvents(uintptr_t ident, int16_t filter, uint16_t flags,
				uint32_t fflags, intptr_t data, void* udata);
		bool	isServerSocket(int fd);
		bool	isClientSocket(int fd);
		int		waitEvent(void);
		void	communicateSock(int newEvents);
		int		readEvent(struct kevent* curEvent);
		void	writeEvent(struct kevent* curEvent);
		void	disconnectClient(int fd);

	public:
		// Orthodox Canonical Form
		server(const std::vector<configInfo>& conf);
		~server();
		server(const server& copy);
		server& operator=(const server& copy);

		void		createServerSock(void);
		void 		run(void);
};
#endif //server_hpp
