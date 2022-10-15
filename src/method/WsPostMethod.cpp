#include "WsPostMethod.hpp"

WsPostMethod::WsPostMethod(const std::string& readLine)
	:WsIMethod(readLine)
{
	m_isBody = false;
	m_bodyBuffer.clear();
}

WsPostMethod::~WsPostMethod()
{}

void
WsPostMethod::loadRequest(const std::string &readLine)
{
	std::cout << "post load request function" << std::endl;
	if (readLine[0] == '\r')
		m_isBody = true;
	if (readLine[0] == ' ')
		return ;
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
	std::cout << m_bodyBuffer << std::endl;
}
