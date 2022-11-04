#include "deleteMethod.hpp"

deleteMethod::deleteMethod(const std::string& readLine, const configInfo& conf)
	:AMethod(readLine, conf)
{}

deleteMethod::~deleteMethod()
{
}

void
deleteMethod::loadRequest(const std::string &readLine)
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
deleteMethod::checkMethodLimit(const std::vector<std::string>& limitExcept) const
{
	for (size_t i = 0; i < limitExcept.size(); i++)
	{
		if (limitExcept[i] == "DELETE")
			return (true);
	}
	return (false);
}

bool
deleteMethod::isMethodCreateFin(void) const
{
	if (m_crlfCnt == 1)
		return (true);
	return (false);
}

void
deleteMethod::logMethodInfo(std::fstream& logFile) const
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
deleteMethod::uriParse(void)
{
	std::vector<std::string>	limitExcept;
	std::vector<std::string>	indexFile;
	std::string					root;
	std::string					uri;
	std::string					fileName;
	std::string					locationPath;

	std::vector<std::string>	directoryVec;
	int							directoryIdx;
	bool						isTrailingSlash;

	uri = m_uri;
	if (uri.back() == '/')
		isTrailingSlash = true;
	else
		isTrailingSlash = false;

	directoryParse(uri, directoryVec);
	directoryIdx = m_conf.isLocationBlock(directoryVec);
	m_conf.findLocation(directoryVec[directoryIdx], root, indexFile, limitExcept);
	fileName = uri.substr(directoryVec[directoryIdx].size());
	if (!isTrailingSlash)
	{
		if (checkFileExists(root + fileName))
		{}
		else if (checkDirExists(root + fileName))
		{
			root = root + fileName + "/";
			fileName = "";
		}
		else
		{
			directoryVec.clear();
			uri.push_back('/');
			directoryParse(uri, directoryVec);
			directoryIdx = m_conf.isLocationBlock(directoryVec);
			if (directoryIdx != 0)
			{
				m_conf.findLocation(directoryVec[directoryIdx], root, indexFile, limitExcept);
				fileName = uri.substr(directoryVec[directoryIdx].size());
			}
		}
	}
	else
	{
		root = root + fileName;
		fileName = "";
	}
	if (fileName == "")
		fileName = indexFile[0];
	m_filePath = root + fileName;
	m_statusCode = 200;

	if (!checkFileExists(m_filePath))
	{
		m_filePath = m_conf.getErrorPath();
		m_statusCode = 404;
		m_filePath.replace(m_filePath.find('*'), 1, std::to_string(m_statusCode));
		return ;
	}
	if (!this->checkMethodLimit(limitExcept))
	{
		m_filePath = m_conf.getErrorPath();
		m_statusCode = 405;
		m_filePath.replace(m_filePath.find('*'), 1, std::to_string(m_statusCode));
	}

}

