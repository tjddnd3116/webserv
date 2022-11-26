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
deleteMethod::isMethodCreateFin(void)
{
	if (m_crlfCnt == 1)
	{
		this->uriParse();
		return (true);
	}
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
	std::string					uri;

	uri = m_uri;
	this->filePathParse(uri);
	m_statusCode = 204;
}

void
deleteMethod::doMethodWork(void)
{
	int deleteStatus;
	std::vector<std::string> limitExcept;


	limitExcept = m_location->locLimitExpect;
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
	readFile(m_readBody);
	deleteStatus = unlink(m_filePath.c_str());
	if (deleteStatus == -1)
		m_statusCode = 202;
}

std::string&
deleteMethod::getReadBody(void)
{
	return (m_readBody);
}

void
deleteMethod::filePathParse(std::string uri)
{
	std::vector<std::string>	directoryVec;
	std::vector<std::string>	indexFile;
	std::string					fileName;
	std::string					root;
	bool						isTrailingSlash;
	int							directoryIdx;

	isTrailingSlash = getTrailingSlash(uri);
	directoryParse(uri, directoryVec);
	directoryIdx = m_conf.isLocationBlock(directoryVec);
	m_location = m_conf.findLocation(directoryVec[directoryIdx]);
	if (m_location->locAlias != "")
		root = m_location->locAlias + "/";
	else
		root= m_location->locRoot + directoryVec[directoryIdx];
	indexFile = m_location->locIndex;
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
				m_location = m_conf.findLocation(directoryVec[directoryIdx]);
				if (m_location->locAlias != "")
					root = m_location->locAlias + "/";
				else
					root= m_location->locRoot + directoryVec[directoryIdx];
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
	extractExt(fileName);
	m_filePath = root + fileName;
}
