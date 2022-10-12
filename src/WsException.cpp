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
WsException::printConfigErr(const std::vector<std::string>& configFile)
{
	for (size_t i = 0; i < configFile.size(); i++)
	{
		std::cout << i + 1;
		std::cout << " ";
		if (m_errLine == i)
		{
			std::cout << RED << configFile[i];
			std::cout << "\t\t<< error occured";
			std::cout << " [" << m_errMsg << "]" << NC << std::endl;
		}
		else
			std::cout << configFile[i] << std::endl;
	}
}
