#ifndef server_hpp
#define server_hpp

#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <map>
#include <vector>
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

		std::vector<struct kevent>	m_changeList;
		struct kevent				m_eventList[EVENT_SIZE];
		int							m_kq;

		std::map<int, serverSocket>	m_serverSock;
		std::map<int, clientSocket>	m_clientSock;

		// private memeber functions

		// event 등록 wrapper 함수
		void	addEvents(uintptr_t ident, int16_t filter, uint16_t flags,
				uint32_t fflags, intptr_t data, void* udata);
		// event 대기 함수
		int		waitEvent(void);
		// event 발생시 event의 종류확인, 해당 event 처리와 예외상황 발생시 throw
		void	communicateSock(int newEvents);
		// read event 발생시 처리
		int		readEvent(struct kevent* curEvent);
		// write event 발생시 처리
		void	writeEvent(struct kevent* curEvent);
		// 해당 fd의 socket disconnect
		void	disconnectClientSocket(int fd);
		// 해당 fd가 server socket인지 판별
		bool	isServerSocket(int fd);
		// 해당 fd가 client socket인지 판별
		bool	isClientSocket(int fd);

		// hide copy constructor & operator
		server(const server& copy);
		server& operator=(const server& copy);

	public:
		// constructor & destructor
		server(const std::vector<configInfo>& conf, std::fstream& logFile);
		~server();

		// public memeber functions

		/* config파일의 server block개수만큼
		 *
		 * server socket을 생성하고 m_serverSock에 저장한다.*/
		void		createServerSock(void);

		/* server를 작동시킨다.*/
		void 		run(void);
};

#endif //server_hpp
