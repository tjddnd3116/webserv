#include "getMethod.hpp"

getMethod::getMethod(const std::string& readLine, const configInfo& conf)
	:AMethod(readLine, conf)
{}

getMethod::~getMethod()
{

}

void
getMethod::loadRequest(const std::string &readLine)
{
	if (readLine[0] == ' ')
		return ;
	std::vector<std::string> splittedLine(splitReadLine(readLine, ","));
	splittedLine[0].pop_back();
	for (size_t vecIdx = 1; vecIdx < splittedLine.size(); vecIdx++)
		m_requestSet[splittedLine[0]].push_back(splittedLine[vecIdx]);
}

void getMethod::printBody(void) const
{
	std::cout << RED << "-------body--------" << std::endl;
	std::cout << "no body" << std::endl;
	std::cout << "-------------------" << RESET << std::endl;
}