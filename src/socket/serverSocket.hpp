#ifndef serverSocket_hpp
#define serverSocket_hpp

#include "ASocket.hpp"

class serverSocket : public ASocket
{
	public:
		// constructor && destructor
		serverSocket(const configInfo& conf);
		~serverSocket();

		// virtual member functions

		// 새로운 server socket을 생성한다.
		virtual void	createSock(void);

		// public member functions

		// socketAddr에 port번호를 저장한다.
		void			initAddr();
		// port 번호를 저장한 socketAddr와 server socket과 bind를 한다.
		void			bindSock(void);
		// server socket을 listening 상태로 만든다.
		void			listenSock(void);
};
#endif //serverSocket_hpp
