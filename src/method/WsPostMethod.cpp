#include "WsPostMethod.hpp"

WsPostMethod::WsPostMethod(const std::string& readLine, const WsConfigInfo& conf)
	:WsIMethod(readLine, conf)
{
	m_isBody = false;
	m_bodyBuffer.clear();
}

WsPostMethod::~WsPostMethod()
{}

void
WsPostMethod::loadRequest(const std::string &readLine)
{
	if (readLine[0] == ' ')
		return ;
	if (readLine[0] == '\r')
	{
		m_isBody = true;
		return;
	}
	if (m_isBody)
		return (loadBody(readLine));
	std::vector<std::string> splittedLine(splitReadLine(readLine, ","));
	splittedLine[0].pop_back();
	for (size_t vecIdx = 1; vecIdx < splittedLine.size(); vecIdx++)
		m_requestSet[splittedLine[0]].push_back(splittedLine[vecIdx]);
}

void
WsPostMethod::loadBody(const std::string& readLine)
{
	m_bodyBuffer += readLine;
}

void
WsPostMethod::printBody(void) const
{
	std::cout << RED << "-------body--------" << std::endl;
	std::cout << m_bodyBuffer << std::endl;
	std::cout << "-------------------" << RESET << std::endl;
}
