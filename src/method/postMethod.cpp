#include "postMethod.hpp"

postMethod::postMethod(const std::string& readLine, const configInfo& conf)
	:AMethod(readLine, conf)
{
	m_isBody = false;
	m_bodyBuffer.clear();
}

postMethod::~postMethod()
{}

void
postMethod::loadRequest(const std::string &readLine)
{
	if (!m_isBody && readLine.empty())
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

const std::string&
postMethod::getBody(void) const
{
	return (m_bodyBuffer);
}

void
postMethod::loadBody(const std::string& readLine)
{
	m_bodyBuffer += readLine + "\n";
}

void
postMethod::printBody(void) const
{
	std::cout << RED << "-------body--------" << std::endl;
	std::cout << m_bodyBuffer << std::endl;
	std::cout << "-------------------" << RESET << std::endl;
}

bool
postMethod::checkMethodLimit(const std::vector<std::string>& limitExcept) const
{
	for (size_t i = 0; i < limitExcept.size(); i++)
	{
		if (limitExcept[i] == "POST")
			return (true);
	}
	return (false);
}
