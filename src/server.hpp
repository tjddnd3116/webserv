#ifndef server_hpp
#define server_hpp

#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <map>
#include <vector>
// #include <iostream>
#include <fstream>

#include "./parser/configInfo.hpp"
#include "./socket/clientSocket.hpp"
#include "./socket/serverSocket.hpp"
#include "./socket/response.hpp"

#define EVENT_SIZE 8

class server
{
	private:
		// member variable
		std::vector<configInfo>		m_conf;
		std::fstream&				m_logFile;
		//
		//

		std::map<int, serverSocket>	m_serverSock;
		std::map<int, clientSocket>	m_clientSock;
		std::vector<struct kevent>	m_changeList;
		struct kevent				m_eventList[EVENT_SIZE];
		int							m_kq;
		size_t						m_serverSize;
		AMethod*					m_method;

		void	addEvents(uintptr_t ident, int16_t filter, uint16_t flags,
				uint32_t fflags, intptr_t data, void* udata);
		bool	isServerSocket(int fd);
		bool	isClientSocket(int fd);
		int		waitEvent(void);
		void	communicateSock(int newEvents);
		int		readEvent(struct kevent* curEvent);
		void	writeEvent(struct kevent* curEvent);
		void	disconnectClient(int fd);

		// hide copy
		server(const server& copy);
		server& operator=(const server& copy);

	public:
		// constructor & destructor
		server(const std::vector<configInfo>& conf, std::fstream& logFile);
		~server();

		void		createServerSock(void);
		void 		run(void);
};
#endif //server_hpp
