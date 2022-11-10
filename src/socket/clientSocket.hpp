#ifndef clientSocket_hpp
#define clientSocket_hpp

#include "../parser/configInfo.hpp"
#include "ASocket.hpp"
#include "request.hpp"
#include "response.hpp"

class clientSocket : public ASocket
{
	private:
		response*	m_responsePtr;
		AMethod*	m_method;
		request		m_request;
		size_t		m_sentSize;

		std::string	m_readBuffer;
		bool		m_readFinish;
		bool		m_writeFinish;

	public:
		// constructor & destructor
		clientSocket(const ASocket& serverSock);
		~clientSocket();

		// copy constructor & operator
		clientSocket(const clientSocket& copy);
		clientSocket& operator=(const clientSocket& copy);

		// public member functions
		void	createSock(void);
		int		readSock(std::fstream& logFile, int msgSize);
		int		sendSock(std::fstream& logFile);

		// public getter functions
		bool	getReadStatus(void) const;
		bool 	getWriteStatus(void) const;
};
#endif //clientSocket_hpp
