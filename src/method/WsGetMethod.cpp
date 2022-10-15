#include "WsGetMethod.hpp"

WsGetMethod::WsGetMethod(const std::string& readLine)
	:WsIMethod(readLine)
{}

WsGetMethod::~WsGetMethod()
{

}

void
WsGetMethod::loadRequest(const std::string &readLine)
{
	if (readLine[0] == ' ')
		return ;
	std::vector<std::string> splittedLine(splitReadLine(readLine, ","));
	splittedLine[0].pop_back();
	for (size_t vecIdx = 1; vecIdx < splittedLine.size(); vecIdx++)
		m_requestSet[splittedLine[0]].push_back(splittedLine[vecIdx]);
}

void WsGetMethod::printBody(void) const
{
	std::cout << "no body" << std::endl;
}
