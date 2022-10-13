#ifndef WsClientSock_hpp
#define WsClientSock_hpp

#include "../parser/WsConfigInfo.hpp"
#include "WsASocket.hpp"
#include "WsRequest.hpp"
#include "WsResponse.hpp"

class WsClientSock : public WsASocket
{
	private:
		WsIMethod*	m_method;
		std::string	m_readBuffer;
		bool		m_readFinish;

	public:
		WsClientSock(const WsConfigInfo& conf);
		WsClientSock(const WsASocket& serverSock);
		~WsClientSock();
		WsClientSock(const WsClientSock& copy);
		WsClientSock& operator=(const WsClientSock& copy);

		void	createSock(void);
		int		readSock(void);
		int		sendSock(void);
		bool	getReadStatus(void) const;
};
#endif //WsClientSock_hpp
