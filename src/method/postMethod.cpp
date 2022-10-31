#include "postMethod.hpp"
#include <string>

postMethod::postMethod(const std::string& readLine, const configInfo& conf)
	:AMethod(readLine, conf)
{
	m_bodyBuffer.clear();
}

postMethod::~postMethod()
{}

void
postMethod::loadRequest(const std::string &readLine)
{
	if (readLine.empty() || readLine[0] == ' ')
		return ;
	if (readLine[0] == '\r')
	{
		m_crlfCnt++;
		getBodySize();
		return ;
	}
	if (m_crlfCnt == 1)
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
	if (m_bodyBuffer.empty())
		m_bodyBuffer += readLine;
	else
		m_bodyBuffer += "\n" + readLine;
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

bool
postMethod::isMethodCreateFin(void) const
{
	if (m_bodySize == m_bodyBuffer.size())
		return (true);
	if (m_crlfCnt == 2)
		return (true);
	return (false);
}

void postMethod::logMethodInfo(std::fstream& logFile) const
{
	logFile << RED;
	logFile << "---- request message -----" << std::endl;
	logFile << "method :" << std::endl;
	logFile << "\t" << m_methodType << std::endl;
	logFile << "uri :" << std::endl;
	logFile << "\t" << m_uri << std::endl;
	logFile << "http version : " << std::endl;
	logFile << "\t" << m_httpVersion << std::endl;

	std::map<std::string, std::vector<std::string> >::const_iterator mapIt;
	mapIt = m_requestSet.begin();
	for (; mapIt != m_requestSet.end(); mapIt++)
	{
		logFile << mapIt->first << " :"<< std::endl;
		for (size_t setIdx = 0; setIdx < mapIt->second.size(); setIdx++)
			logFile << "\t" << mapIt->second.at(setIdx) << std::endl;
	}
	logFile << "-------body--------" << std::endl;
	logFile << m_bodyBuffer << std::endl;
	logFile << "-------------------------" << RESET << std::endl;
}

void
postMethod::getBodySize(void)
{
	std::map<std::string, std::vector<std::string> >::iterator mapIt;

	mapIt = m_requestSet.find("content-length");
	m_bodySize = std::stoi(mapIt->second[0]);
}
