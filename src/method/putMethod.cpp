#include "putMethod.hpp"

putMethod::putMethod(const std::string& readLine, const configInfo& conf)
	:AMethod(readLine, conf)
{
	m_bodyBuffer.clear();
	m_bodySize = -1;
	m_readBody = "";
	m_readLineSize = 0;
}

putMethod::~putMethod()
{}

void
putMethod::loadRequest(const std::string &readLine)
{
	if (readLine.empty() || readLine[0] == ' ')
		return ;
	if (readLine[0] == '\r')
	{
		m_crlfCnt++;
		if (m_crlfCnt == 1)
		{
			getBodyType();
			this->uriParse();
		}
		return ;
	}
	if (m_crlfCnt == 1)
		return (loadBody(readLine));
	std::vector<std::string> splittedLine(splitReadLine(readLine, ","));
	splittedLine[0].pop_back();
	for (size_t vecIdx = 1; vecIdx < splittedLine.size(); vecIdx++)
		m_requestSet[splittedLine[0]].push_back(splittedLine[vecIdx]);
}

// std::string&
// putMethod::getBody(void)
// {
//     return (m_bodyBuffer);
// }

void
putMethod::loadBody(const std::string& readLine)
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
			{
				m_bodyBuffer += readLine;
				m_readLineSize += readLine.size();
			}
			if (m_bodySize == m_readLineSize)
			{
				m_bodySize = -1;
				m_readLineSize = 0;
			}
		}
	}
}

bool
putMethod::checkMethodLimit(const std::vector<std::string>& limitExcept) const
{
	for (size_t i = 0; i < limitExcept.size(); i++)
	{
		if (limitExcept[i] == "PUT")
			return (true);
	}
	return (false);
}

bool
putMethod::isMethodCreateFin(void)
{
	if (m_crlfCnt == 2)
		return (true);
	else if (m_bodyType == "size" && m_bodyBuffer.size() == (size_t)m_bodySize)
		return (true);
	return (false);
}

void putMethod::logMethodInfo(std::fstream& logFile) const
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
putMethod::getBodyType(void)
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
putMethod::uriParse(void)
{
	std::string					uri;

	uri = m_uri;
	this->filePathParse(uri);
	m_statusCode = 200;
}

void putMethod::doMethodWork(void)
{
	std::vector<std::string> limitExcept;

	limitExcept = m_location->locLimitExpect;
	if (!this->checkMethodLimit(limitExcept))
	{
		m_filePath = m_conf.getErrorPath();
		m_statusCode = 405;
		m_filePath.replace(m_filePath.find('*'), 1, std::to_string(m_statusCode));
		readFile(m_readBody);
		return;
	}
	writeFile(m_bodyBuffer);
}

std::string&
putMethod::getReadBody(void)
{
	return (m_readBody);
}

void
putMethod::filePathParse(std::string uri)
{
	std::vector<std::string>	directoryVec;
	std::string					root;
	std::string					fileName;
	int							directoryIdx;

	directoryParse(uri, directoryVec);
	directoryIdx = m_conf.isLocationBlock(directoryVec);
	m_location = m_conf.findLocation(directoryVec[directoryIdx]);
	if (m_location->locAlias != "")
		root = m_location->locAlias + "/";
	else
		root= m_location->locRoot + directoryVec[directoryIdx];
	fileName = uri.substr(directoryVec[directoryIdx].size());
	extractExt(fileName);
	m_filePath = root + fileName;
}
