#include "WsIMethod.hpp"

WsIMethod::WsIMethod(const std::string& readLine)
{
	std::vector<std::string> splittedLine;

	splittedLine = splitReadLine(readLine);
	m_method = splittedLine[0];
	m_uri = splittedLine[1];
	m_httpVersion = splittedLine[2];
}

WsIMethod::~WsIMethod()
{}

void WsIMethod::loadRequest(const std::string& readLine)
{
	std::vector<std::string> splittedLine(splitReadLine(readLine, ","));
	for (size_t vecIdx = 1; vecIdx < splittedLine.size(); vecIdx++)
		m_requestSet[splittedLine[0]].push_back(splittedLine[vecIdx]);
}

std::vector<std::string>
WsIMethod::splitReadLine(const std::string& readLine, const std::string& str)
{
	std::vector<std::string> splittedLine;
	size_t prePos = 0;
	size_t curPos = 0;

	curPos = readLine.find(" ", 0);
	while (curPos != std::string::npos)
	{
		std::string tmpStr = readLine.substr(prePos, curPos - prePos);
		splittedLine.push_back(tmpStr);
		prePos = curPos + 1;
		curPos = readLine.find(str, prePos);
	}
	splittedLine.push_back(readLine.substr(prePos, readLine.size() - prePos));
	return (splittedLine);
}

void WsIMethod::printInfo(void) const
{
	std::cout << RED;
	std::cout << "---- request message -----" << std::endl;
	std::cout << "method :" << std::endl;
	std::cout << "\t" << m_method << std::endl;
	std::cout << "uri :" << std::endl;
	std::cout << "\t" << m_uri << std::endl;
	std::cout << "http version : " << std::endl;
	std::cout << "\t" << m_httpVersion << std::endl;

	std::map<std::string, std::vector<std::string> >::const_iterator mapIt;
	mapIt = m_requestSet.begin();
	for (; mapIt != m_requestSet.end(); mapIt++)
	{
		std::cout << mapIt->first << std::endl;
		for (size_t setIdx = 0; setIdx < mapIt->second.size(); setIdx++)
			std::cout << "\t" << mapIt->second.at(setIdx) << std::endl;
	}
	std::cout << "-------------------------" << RESET << std::endl;
}

const std::string& WsIMethod::getUri(void) const
{
	return (m_uri);
}

const std::string& WsIMethod::getHttpVersion(void) const
{
	return (m_httpVersion);
}

const std::map<std::string, std::vector<std::string> >&
WsIMethod::getRequestSet(void) const
{
	return (m_requestSet);
}


