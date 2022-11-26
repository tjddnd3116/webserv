#include "postMethod.hpp"
#include "../cgi/cgi.hpp"

postMethod::postMethod(const std::string& readLine, const configInfo& conf)
	:AMethod(readLine, conf)
{
	m_bodySize = -1;
	m_readBody = "";
	m_readLineSize = 0;
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
		{
			getBodyType();
			this->uriParse();
			launchCgi();
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

void
postMethod::loadBody(const std::string& readLine)
{
	if (m_fileExt == "" || m_location->locCgiExt != m_fileExt)
	{
		if (m_bodyType == "size")
		{
			if (m_readBody.empty())
				m_readBody += readLine;
			else
				m_readBody += "\n" + readLine;
		}
		else if (m_bodyType == "chunked")
		{
			if (m_bodySize == -1)
				m_bodySize = hexToDecimal(readLine);
			else
			{
				if (m_bodySize != 0)
				{
					m_readBody += readLine;
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
	else
	{
		if (m_bodySize == m_readLineSize)
		{
			int decimal = hexToDecimal(readLine);

			m_bodySize += decimal;
			if (decimal == 0)
			{
				m_cgi->writeCgi(m_tempBuffer.data(), m_tempBuffer.size());
				m_cgi->closeCgi(WRITE);
				m_readBody += m_cgi->readCgi();
			}
			return;
		}
		if (m_bodySize == -1)
			m_bodySize = hexToDecimal(readLine);
		else
		{
			if (m_bodySize != 0)
			{
				m_tempBuffer += readLine;
				m_readLineSize += readLine.size();
			}
			if (m_bodySize == m_readLineSize)
			{
				if (m_bodySize < 4097 && m_fileExt == ".bla")
					return;
				m_cgi->writeCgi(m_tempBuffer.data(), m_tempBuffer.size());
				m_readBody += m_cgi->readCgi();
				m_tempBuffer.clear();
				if (m_bodyType == "chunked")
					m_bodySize = -1;
				else
				{
					m_crlfCnt = 2;
				}
				m_readLineSize = 0;
			}
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
postMethod::isMethodCreateFin(void)
{
	if (m_crlfCnt == 2)
	{
		if (m_cgi != NULL)
		{
			m_cgi->closeCgi(READ);
			delete m_cgi;
			m_cgi = NULL;
		}
		return (true);
	}
	else if (m_bodyType == "size" && m_readBody.size() == (size_t)m_bodySize)
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
	logFile << "-------------------------" << RESET << std::endl;
}

void
postMethod::getBodyType(void)
{
	std::map<std::string, std::vector<std::string> >::iterator mapIt;

	mapIt = m_requestSet.find("Content-Length");
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
	this->filePathParse(uri);
	m_statusCode = 200;
}

void postMethod::doMethodWork(void)
{
	std::vector<std::string>	limitExcept;
	int32_t						maxBodySize;

	limitExcept = m_location->locLimitExpect;
	maxBodySize = m_location->clientMaxBodySize;
	if (!this->checkMethodLimit(limitExcept))
	{
		m_filePath = m_conf.getErrorPath();
		m_statusCode = 405;
		m_filePath.replace(m_filePath.find('*'), 1, std::to_string(m_statusCode));
		readFile(m_readBody);
		return ;
	}
	if (maxBodySize != -1 && m_readBody.size() > (size_t)maxBodySize)
	{
		m_filePath = m_conf.getErrorPath();
		m_statusCode = 413;
		m_filePath.replace(m_filePath.find('*'), 1, std::to_string(m_statusCode));
		readFile(m_readBody);
		return ;
	}
}

std::string&
postMethod::getReadBody(void)
{
	return (m_readBody);
}

void
postMethod::filePathParse(std::string uri)
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
