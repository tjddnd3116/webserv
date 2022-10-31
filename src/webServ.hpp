#ifndef webServ_hpp
#define webServ_hpp

#include <fstream>
#include <string>
#include <vector>

#include "server.hpp"
#include "WsException.hpp"
#include "./parser/configInfo.hpp"
#include "./parser/parser.hpp"

#define ERR_LOG_FILE_PATH "/Users/gson/webserv_cgi/errlog.txt"

class webServ
{
	private:
		std::vector<configInfo> m_configInfo;
		std::fstream			m_logFile;

		webServ(const webServ& copy);
		webServ& operator=(const webServ& copy);
	public:
		webServ();
		~webServ();

		void	parsing(const char* configFilePath);
		void	serverRun(void);
		void	clear(void);
};
#endif //webServ_hpp
