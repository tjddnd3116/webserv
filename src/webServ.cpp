#include "webServ.hpp"
#include "WsException.hpp"

webServ::webServ()
{
	m_logFile.open(ERR_LOG_FILE_PATH);
	if (m_logFile.fail())
		throw (WsException("log file open error"));
	m_logFile << "";
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
		throw (WsException("check log file [" ERR_LOG_FILE_PATH "]"));
	}
}

void
webServ::serverRun(void)
{
	server server(m_configInfo);

	server.createServerSock();
	server.run();
}

void
webServ::clear(void)
{}
