#ifndef WsServerSock_hpp
#define WsServerSock_hpp

#include "WsASocket.hpp"

class WsServerSock : public WsASocket
{
	public:
		WsServerSock(const WsConfigInfo& conf);
		~WsServerSock();

		virtual void	createSock(void);
		void			initAddr();
		void			bindSock(void);
		void			listenSock(void);
};
#endif //WsServerSock_hpp
