#include "AMethod.hpp"
#include <cctype>
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

bool
AMethod::checkDirExists(const std::string& filePath)
{
	struct stat buffer;
	int			exist;

	exist = stat(filePath.c_str(), &buffer);
	if (exist == 0 && ((buffer.st_mode & S_IFMT) == S_IFDIR))
		return (true);
	return (false);

}

int
AMethod::hexToDecimal(const std::string& readLine)
{
	int decimal;

	decimal = 0;
	for (size_t i = 0; i < readLine.size(); i++)
	{
		decimal *= 16;
		if (std::isdigit(readLine[i]))
			decimal += readLine[i] - '0';
		else
			decimal += readLine[i] - 'a' + 10;
	}
	return (decimal);
}

int
AMethod::getStatusCode(void) const
{
	return (m_statusCode);
}

void
AMethod::queryStringParse(std::string& uri)
{
	size_t queryStringPos;

	queryStringPos = uri.find("?");

	if (queryStringPos != std::string::npos)
	{
		m_queryString.assign(uri, queryStringPos + 1, std::string::npos);
		uri = uri.substr(0, queryStringPos);
	}
}

void
AMethod::directoryParse(std::string& uri,
						std::vector<std::string>& dirVec)
{
	size_t 			slashPos = 0;

	slashPos = uri.find("/", slashPos);
	while (slashPos != std::string::npos)
	{
		dirVec.push_back(uri.substr(0, ++slashPos));
		slashPos = uri.find("/", slashPos);
	}
}

void
AMethod::extractExt(std::string& fileName)
{
	size_t dotPos;

	dotPos = fileName.find(".");
	m_fileExt = "";
	if (dotPos != std::string::npos)
		m_fileExt = fileName.substr(dotPos);
}

const std::string&
AMethod::getFileExt(void) const
{
	return (m_fileExt);
}

