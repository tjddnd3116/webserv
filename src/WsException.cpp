#include "WsException.hpp"

WsException::WsException(const std::string &errMsg)
	:m_errMsg("Error : " + errMsg)
{}

WsException::WsException(size_t lineNum, const std::string &errMsg)
{
	m_errMsg = errMsg;
	m_errLine = lineNum;
}

WsException::~WsException() throw()
{}

const char*
WsException::what() const throw()
{
	return (m_errMsg.c_str());
}

void
WsException::printConfigErr(const std::vector<std::string>& configFile,
							std::fstream& logFile)
{
	for (size_t i = 0; i < configFile.size(); i++)
	{
		logFile << i + 1;
		logFile << " ";
		if (m_errLine == i)
		{
			logFile << RED << configFile[i];
			logFile << "\t\t<< error occured";
			logFile << " [" << m_errMsg << "]" << NC << std::endl;
		}
		else
			logFile << configFile[i] << std::endl;
	}
}

const std::string&
WsException::getErrorMsg(void) const
{
	return (m_errMsg);
}
