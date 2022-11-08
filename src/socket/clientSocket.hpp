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
		request		m_request;
		size_t		m_sentSize;


		std::string	m_readBuffer;
		bool		m_readFinish;
		bool		is_bodySection;

	public:
		clientSocket(const configInfo& conf);
		clientSocket(const ASocket& serverSock);
		~clientSocket();
		clientSocket(const clientSocket& copy);
		clientSocket& operator=(const clientSocket& copy);

		void	createSock(void);
		int		readSock(std::fstream& logFile, int msgSize);
		int		sendSock(std::fstream& logFile);
		bool	getReadStatus(void) const;
		void	sendFinished(void);
};
#endif //clientSocket_hpp
