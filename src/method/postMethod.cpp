#include "postMethod.hpp"

postMethod::postMethod(const std::string& readLine, const configInfo& conf)
	:AMethod(readLine, conf)
{
	m_bodyBuffer.clear();
	m_bodySize = -1;
	m_readBody = "";
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
		if (m_crlfCnt == 1)
			getBodyType();
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
	if (m_bodyType == "size")
	{
		if (m_bodyBuffer.empty())
			m_bodyBuffer += readLine;
		else
			m_bodyBuffer += "\n" + readLine;
	}
	else if (m_bodyType == "chunked")
	{
		if (m_bodySize == -1)
			m_bodySize = hexToDecimal(readLine);
		else
		{
			if (m_bodySize != 0)
				m_bodyBuffer += readLine;
			if ((size_t)m_bodySize == m_bodyBuffer.size())
				m_bodySize = -1;
		}
	}
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
	// logFile << "-------body--------" << std::endl;
	// logFile << m_bodyBuffer << std::endl;
	// logFile << "-------------------------" << RESET << std::endl;
}

void
postMethod::getBodyType(void)
{
	std::map<std::string, std::vector<std::string> >::iterator mapIt;

	mapIt = m_requestSet.find("content-length");
	if (mapIt != m_requestSet.end())
	{
		m_bodySize = std::stoi(mapIt->second[0]);
		m_bodyType = "size";
		return ;
	}
	mapIt = m_requestSet.find("Transfer-Encoding");
	if (mapIt != m_requestSet.end())
	{
		m_bodyType = mapIt->second[0];
		m_bodySize = -1;
		return ;
	}
}

void
postMethod::uriParse(void)
{
	std::string	uri;

	uri = m_uri;
	postFilePathParse(uri);
	m_statusCode = 200;

}

void postMethod::doMethodWork(void)
{
	if (!this->checkMethodLimit(m_limitExcept))
	{
		m_filePath = m_conf.getErrorPath();
		m_statusCode = 405;
		m_filePath.replace(m_filePath.find('*'), 1, std::to_string(m_statusCode));
		readFile(m_readBody);
		return ;
	}
	// TODO
	// need error control
	writeFile(m_bodyBuffer);
}

const std::string&
postMethod::getReadBody(void) const
{
	return (m_readBody);
}
