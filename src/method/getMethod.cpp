#include "getMethod.hpp"

getMethod::getMethod(const std::string& readLine, const configInfo& conf)
	:AMethod(readLine, conf)
{}

getMethod::~getMethod()
{}

void
getMethod::loadRequest(const std::string &readLine)
{
	if (readLine.empty() || readLine[0] == ' ')
		return ;
	if (readLine[0] == '\r')
	{
		m_crlfCnt++;
		return ;
	}
	std::vector<std::string> splittedLine(splitReadLine(readLine, ","));
	splittedLine[0].pop_back();
	for (size_t vecIdx = 1; vecIdx < splittedLine.size(); vecIdx++)
		m_requestSet[splittedLine[0]].push_back(splittedLine[vecIdx]);
}

bool
getMethod::checkMethodLimit(const std::vector<std::string>& limitExcept) const
{
	for (size_t i = 0; i < limitExcept.size(); i++)
	{
		if (limitExcept[i] == "GET")
			return (true);
	}
	return (false);
}

bool
getMethod::isMethodCreateFin(void)
{
	if (m_crlfCnt == 1)
	{
		this->uriParse();
		return (true);
	}
	return (false);
}

void
getMethod::logMethodInfo(std::fstream& logFile) const
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
	logFile << "-------------------------" << RESET << std::endl;
}

void
getMethod::uriParse(void)
{
	std::string	uri;

	uri = m_uri;
	queryStringParse(uri);
	filePathParse(uri);
	m_statusCode = 200;
}

void getMethod::doMethodWork(void)
{
	if (!checkFileExists(m_filePath))
	{
		m_filePath = m_conf.getErrorPath();
		m_statusCode = 404;
		m_filePath.replace(m_filePath.find('*'), 1, std::to_string(m_statusCode));
	}
	if (!this->checkMethodLimit(m_limitExcept))
	{
		m_filePath = m_conf.getErrorPath();
		m_statusCode = 405;
		m_filePath.replace(m_filePath.find('*'), 1, std::to_string(m_statusCode));
	}
	// TODO
	// need error control
	readFile(m_readBody);
}

const std::string&
getMethod::getReadBody(void) const
{
	return (m_readBody);
}
