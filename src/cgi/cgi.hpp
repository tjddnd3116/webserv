#ifndef cgi_hpp
#define cgi_hpp

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "../method/AMethod.hpp"

#define READ 0
#define WRITE 1
#define BUFFER_SIZE 65535

class cgi
{
	private:
		std::vector<std::string>	m_env;
		std::vector<char*>			m_envChar;
		std::string					m_cgiPath;
		std::string 				m_path;
		std::string					m_script;
		std::string					m_query;

		int							m_fromCgiToServer[2];
		int							m_fromServerToCgi[2];
		pid_t						m_pid;
		bool						m_bodyFlag;

	public:
		cgi();
		~cgi();
		void		initCgi(const AMethod *method);
		void		runCgi(void);
		std::string	execCgi(const std::string& readLine);
		ssize_t		writeCgi(const void* buf, size_t size);
		void		closeCgi(int pipeEnd);
		std::string readCgi(void);
		std::string	readCloseCgi(void);
};
#endif //cgi_hpp
