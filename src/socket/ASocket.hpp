#ifndef ASocket_hpp
#define ASocket_hpp

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/_endian.h>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

#include "../WsException.hpp"
#include "../parser/configInfo.hpp"

#define BUF_SIZE 65535

#define RESET	"\033[0m"
#define RED		"\033[31m"
#define BLUE    "\033[34m"

class ASocket
{
		protected:
			configInfo			m_conf;
			struct sockaddr_in	m_SocketAddr;
			socklen_t			m_SocketAddrSize;
			int					m_SocketFd;

		public:
			// constructor & destructor
			ASocket(const configInfo& conf);
			virtual ~ASocket();

			// copy constructor & operator
			ASocket(const ASocket& copy);
			ASocket& operator=(const ASocket &copy);

			// public member functions
			virtual void		createSock(void) = 0;
			void				closeSock();
			int					getSocketFd(void) const;
			const configInfo&	getConf(void) const;
};

#endif //ASocket_hpp
