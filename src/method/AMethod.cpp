#include "AMethod.hpp"
#include <string>
#include <sys/stat.h>
#include <unistd.h>

AMethod::AMethod(const std::string& readLine, const configInfo& conf)
{
	m_conf = conf;
	std::vector<std::string> splittedLine;
	m_statusCode = 0;
	m_methodType = "";
	m_uri = "";
	m_httpVersion = "";
	m_crlfCnt = 0;

	splittedLine = splitReadLine(readLine);
	m_statusCode = checkStartLine(splittedLine);
	if (!m_statusCode)
	{
		m_methodType = splittedLine[0];
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

const std::string&
AMethod::getMethod(void) const
{
	return (m_methodType);
}

const std::string&
AMethod::getUri(void) const
{
	return (m_uri);
}

const configInfo&
AMethod::getConfig(void) const
{
	return (m_conf);
}

const std::string&
AMethod::getHttpVersion(void) const
{
	return (m_httpVersion);
}

const std::string&
AMethod::getBody(void) const
{
	return s;
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

	maxUriLen = m_conf.getUriBufferSize();
	if (splittedLine.size() != 3)
		return (400);							// Bad Request
	if (splittedLine[1].size() > maxUriLen)
		return (414);							// URI Too Long
	if (splittedLine[2] != "HTTP/1.1")
		return (400);							// Bad Request
	return (0);
}

const std::string&
AMethod::getFilePath(void) const
{
	return (m_filePath);
}

void
AMethod::uriParse(void)
{
	std::vector<std::string>	limitExcept;
	std::vector<std::string>	indexFile;
	std::string					root;
	std::string					uri;
	std::string					fileName;
	std::string					locationPath;
	size_t						lastSlashPos;
	size_t						queryStringPos;

	uri = m_uri;
	queryStringPos = uri.find("?");
	if (queryStringPos != std::string::npos)
	{
		m_queryString.assign(uri, queryStringPos + 1, std::string::npos);
		uri = uri.substr(0, queryStringPos);
	}
	lastSlashPos = uri.find_last_of("/");

	if (lastSlashPos == 0)
		locationPath.assign(uri, 0, uri.size());
	else
		locationPath.assign(uri, 0, lastSlashPos + 1);

	std::cout << "locationPath is: " << locationPath << std::endl;
	fileName.assign(uri, lastSlashPos, uri.size());
	m_conf.findLocation(locationPath, root, indexFile, limitExcept);
	m_filePath = root;
	std::cout << "fileName is: " << fileName << std::endl;
	std::cout << "indexFile is: " << indexFile[0] << std::endl;
	if (fileName == "/" || locationPath == fileName)
		m_filePath += indexFile[0];
	else
		m_filePath += fileName;
	m_statusCode = 200;
	std::cout << "file path" << m_filePath << std::endl;
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

const std::string&
AMethod::getQueryString(void) const
{
	return (m_queryString);
}

bool
AMethod::checkFileExists(const std::string& filePath)
{
	struct stat buffer;
	int			exist;

	exist = stat(filePath.c_str(), &buffer);
	if (exist == 0 && ((buffer.st_mode & S_IFMT) == S_IFREG))
		return (true);
	return (false);
}

int
AMethod::getStatusCode(void) const
{
	return (m_statusCode);
}
