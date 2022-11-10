#include "webServ.hpp"

webServ::webServ()
{
	umask(0000);
	m_logFile.open(ERR_LOG_FILE_PATH, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
	if (m_logFile.fail())
		throw (WsException("log file open error"));
}

webServ::~webServ()
{
	m_logFile.close();
}

void
webServ::parsing(const char* configFilePath)
{
	try {
	parser parser(configFilePath);

	parser.configParse(m_configInfo, m_logFile);
	} catch (std::exception& e) {
		m_logFile << e.what() << std::endl;
		throw (WsException("parsing error check log file [" ERR_LOG_FILE_PATH "]"));
	}
}

void
webServ::serverRun(void)
{
	try {
		server server(m_configInfo, m_logFile);

		server.createServerSock();
		server.run();
	} catch (std::exception& e) {
		m_logFile << e.what() << std::endl;
		throw (WsException("server error check log file [" ERR_LOG_FILE_PATH "]"));
	}
}

void
webServ::clear(void)
{}
