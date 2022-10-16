#include "AMethod.hpp"

AMethod::AMethod(const std::string& readLine, const configInfo& conf)
{
	m_conf = conf;
	std::vector<std::string> splittedLine;
	m_statusCode = 0;
	m_method = "";
	m_uri = "";
	m_httpVersion = "";

	splittedLine = splitReadLine(readLine);
	m_statusCode = checkStartLine(splittedLine);
	if (!m_statusCode)
	{
		m_method = splittedLine[0];
		m_uri = splittedLine[1];
		m_httpVersion = splittedLine[2];
	}
}

AMethod::~AMethod()
{}

std::vector<std::string>
AMethod::splitReadLine(const std::string& readLine, const std::string& str)
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

const std::string& AMethod::getUri(void) const
{
	return (m_uri);
}

const std::string& AMethod::getHttpVersion(void) const
{
	return (m_httpVersion);
}

const std::map<std::string, std::vector<std::string> >&
AMethod::getRequestSet(void) const
{
	return (m_requestSet);
}

int
AMethod::checkStartLine(std::vector<std::string>& splittedLine)
{
	uint32_t maxUriLen;

	maxUriLen = std::atoi(m_conf.getUriBufferSize()[0].c_str());
	maxUriLen *= 1024;
	if (splittedLine.size() != 3)
		return (400);							// Bad Request
	if (splittedLine[1].size() > maxUriLen)
		return (414);							// URI Too Long
	if (splittedLine[2] != "HTTP/1.1\r")
		return (400);							// Bad Request
	return (0);
}

std::ostream&
operator<<(std::ostream& os, const AMethod& method)
{
	os << RED;
	os << "---- request message -----" << std::endl;
	os << "method :" << std::endl;
	os << "\t" << method.m_method << std::endl;
	os << "uri :" << std::endl;
	os << "\t" << method.m_uri << std::endl;
	os << "http version : " << std::endl;
	os << "\t" << method.m_httpVersion << std::endl;

	std::map<std::string, std::vector<std::string> >::const_iterator mapIt;
	mapIt = method.m_requestSet.begin();
	for (; mapIt != method.m_requestSet.end(); mapIt++)
	{
		os << mapIt->first << " :"<< std::endl;
		for (size_t setIdx = 0; setIdx < mapIt->second.size(); setIdx++)
			os << "\t" << mapIt->second.at(setIdx) << std::endl;
	}
	os << "-------------------------" << RESET << std::endl;
	return (os);
}
