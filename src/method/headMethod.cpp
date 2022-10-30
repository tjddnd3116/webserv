#include "headMethod.hpp"

headMethod::headMethod(const std::string& readLine, const configInfo& conf)
	:AMethod(readLine, conf)
{}

headMethod::~headMethod()
{}

void
headMethod::loadRequest(const std::string &readLine)
{
	if (readLine.empty() || readLine[0] == ' ')
		return ;
	std::vector<std::string> splittedLine(splitReadLine(readLine, ","));
	splittedLine[0].pop_back();
	for (size_t vecIdx = 1; vecIdx < splittedLine.size(); vecIdx++)
		m_requestSet[splittedLine[0]].push_back(splittedLine[vecIdx]);
}

void
headMethod::printBody(void) const
{
	std::cout << RED << "-------body--------" << std::endl;
	std::cout << "no body" << std::endl;
	std::cout << "-------------------" << RESET << std::endl;
}

bool
headMethod::checkMethodLimit(const std::vector<std::string>& limitExcept) const
{
	for (size_t i = 0; i < limitExcept.size(); i++)
	{
		if (limitExcept[i] == "HEAD")
			return (true);
	}
	return (false);
}
