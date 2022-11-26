#include "AMethod.hpp"

#include <cctype>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include "../cgi/cgi.hpp"

AMethod::AMethod(const std::string& readLine, const configInfo& conf)
{
	m_conf = conf;
	std::vector<std::string> splittedLine;
	m_statusCode = 0;
	m_crlfCnt = 0;
	m_queryString = "";
	m_cgi = NULL;
	m_isCgi = false;
	m_bodySize = -1;

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

// const std::string&
// AMethod::getCgiPath(void) const
// {
//     return (m_cgiPath);
// }
//
// const std::string&
// AMethod::getCgiExt(void) const
// {
//     return (m_cgiExt);
// }

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

const int32_t&
AMethod::getBodySize(void) const
{
	return m_bodySize;
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
	if (uri != "/")
		dirVec.push_back(uri);
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

// void
// AMethod::filePathParse(std::string uri)
// {
//     std::vector<std::string>	directoryVec;
//     std::vector<std::string>	indexFile;
//     std::string					fileName;
//     std::string					root;
//     bool						isTrailingSlash;
//     int							directoryIdx;
//
//
//     isTrailingSlash = getTrailingSlash(uri);
//     directoryParse(uri, directoryVec);
//     directoryIdx = m_conf.isLocationBlock(directoryVec);
//     m_conf.findLocation(directoryVec[directoryIdx], root, indexFile, m_limitExcept, m_maxBodySize, m_cgiPath ,m_cgiExt);
//     fileName = uri.substr(directoryVec[directoryIdx].size());
//     if (!isTrailingSlash)
//     {
//         if (checkFileExists(root + fileName))
//         {}
//         else if (checkDirExists(root + fileName))
//         {
//             root = root + fileName + "/";
//             fileName = "";
//         }
//         else
//         {
//             directoryVec.clear();
//             uri.push_back('/');
//             directoryParse(uri, directoryVec);
//             directoryIdx = m_conf.isLocationBlock(directoryVec);
//             if (directoryIdx != 0)
//             {
//                 m_conf.findLocation(directoryVec[directoryIdx], root, indexFile, m_limitExcept, m_maxBodySize, m_cgiPath ,m_cgiExt);
//                 fileName = uri.substr(directoryVec[directoryIdx].size());
//             }
//         }
//     }
//     else
//     {
//         root = root + fileName;
//         fileName = "";
//     }
//     if (fileName == "")
//         fileName = indexFile[0];
//     extractExt(fileName);
//     m_filePath = root + fileName;
// }

bool
AMethod::getTrailingSlash(const std::string& uri)
{
	if (uri.back() == '/')
		return (true);
	return (false);
}

void
AMethod::readFile(std::string& readBody)
{
	std::fstream	file;
	std::string		readLine;

	file.open(m_filePath);
	while (!file.eof())
	{
		std::getline(file, readLine);
		if (readLine == "")
			continue;
		readBody += readLine + "\n";
	}
	readBody.pop_back();
	file.close();
}

void
AMethod::writeFile(std::string& bodyBuffer)
{
	std::ofstream file(m_filePath);

	if (file.fail())
	{
		std::cout << "open fail!" << std::endl;
		return ;
	}
	file << bodyBuffer;
	file.close();
}

int
AMethod::getCrlfCnt(void) const
{
	return (m_crlfCnt);
}

void
AMethod::makeIndexOf(std::string& readBody)
{
	DIR*			dirPtr = NULL;
	struct dirent*	file = NULL;

	if ((dirPtr = opendir(m_filePath.c_str()))==NULL)
	{
		std::cout << "opendir error" << std::endl;
		exit(1);
	}
	readBody = "Index of " + m_filePath + "\n";
	readBody += "--------------------------\n";
	while ((file = readdir(dirPtr)) != NULL)
	{
		readBody += file->d_name;
		readBody += "\n";
	}
	closedir(dirPtr);
}

configInfo::Location*
AMethod::getLocation(void) const
{
	return (m_location);
}

const std::string&
AMethod::getBodyType(void) const
{
	return (m_bodyType);
}

bool
AMethod::isCgiExt(void)
{
	if (m_fileExt.size() && m_location->locCgiExt == m_fileExt)
	{
		m_cgi = new cgi;
		m_cgi->initCgi(this);
		m_cgi->runCgi();
		return (true);
	}
	return (false);
}

bool
AMethod::getIsCgi(void) const
{
	return (m_isCgi);
}

void
AMethod::launchCgi(void)
{
	if (m_fileExt.size() && m_location->locCgiExt == m_fileExt)
	{
		m_isCgi = true;
		m_cgi = new cgi;
		m_cgi->initCgi(this);
		m_cgi->runCgi();
	}
}
