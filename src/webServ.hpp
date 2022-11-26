#ifndef webServ_hpp
#define webServ_hpp

#include <fstream>
#include <string>
#include <vector>

#include "./parser/configInfo.hpp"
#include "./parser/parser.hpp"
#include "server.hpp"
#include "WsException.hpp"

#define ERR_LOG_FILE_PATH "/dev/null"
// #define ERR_LOG_FILE_PATH "/Users/soum/webserv/log.txt"

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
