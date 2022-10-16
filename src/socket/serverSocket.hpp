#ifndef serverSocket_hpp
#define serverSocket_hpp

#include "ASocket.hpp"

class serverSocket : public ASocket
{
	public:
		serverSocket(const configInfo& conf);
		~serverSocket();

		virtual void	createSock(void);
		void			initAddr();
		void			bindSock(void);
		void			listenSock(void);
};
#endif //serverSocket_hpp
