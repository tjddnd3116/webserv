#ifndef clientSocket_hpp
#define clientSocket_hpp

#include "../parser/configInfo.hpp"
#include "ASocket.hpp"
#include "request.hpp"
#include "response.hpp"

class clientSocket : public ASocket
{
	private:
		AMethod*	m_method;
		std::string	m_readBuffer;
		bool		m_readFinish;

	public:
		clientSocket(const configInfo& conf);
		clientSocket(const ASocket& serverSock);
		~clientSocket();
		clientSocket(const clientSocket& copy);
		clientSocket& operator=(const clientSocket& copy);

		void	createSock(void);
		int		readSock(std::fstream& logFile);
		int		sendSock(std::fstream& logFile);
		bool	getReadStatus(void) const;
};
#endif //clientSocket_hpp
