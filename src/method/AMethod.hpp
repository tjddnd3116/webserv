#ifndef AMethod_hpp
#define AMethod_hpp

#include <fstream>
#include <sys/stat.h>
#include <dirent.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../parser/configInfo.hpp"

#define RESET	"\033[0m"
#define RED		"\033[31m"
#define BLUE    "\033[34m"

class Location;
class cgi;

class AMethod
{
	typedef std::map<std::string, std::vector<std::string> > requestMap;

	protected:
		std::string						m_methodType;
		std::string						m_uri;
		std::string						m_httpVersion;
		requestMap						m_requestSet;
		configInfo						m_conf;
		int								m_statusCode;
		std::string						m_bodyType;
		cgi*							m_cgi;
		int32_t							m_bodySize;

		configInfo::Location*			m_location;
		std::string						m_filePath;
		std::string						m_queryString;
		std::string						m_fileExt;
		int								m_crlfCnt;
		bool							m_isCgi;
		// std::string						m_cgiPath;
		// std::string						m_cgiExt;

		std::vector<std::string>	splitReadLine(const std::string& readLine,
												  const std::string& str = " ");
		int							checkStartLine(std::vector<std::string>& splittedLine);
		void						loadBody(const std::string& readLine);
		bool						checkFileExists(const std::string& filePath);
		bool						checkDirExists(const std::string& filePath);
		int							hexToDecimal(const std::string& readLine);
		void						extractExt(std::string& fileName);
		bool						getTrailingSlash(const std::string& uri);
		void						readFile(std::string& readBody);
		void						writeFile(std::string& bodyBuffer);
		void						makeIndexOf(std::string& readBody);
		bool						isCgiExt(void);

	public:
		// constructor & destructor
		AMethod(const std::string& readLine, const configInfo& conf);
		virtual ~AMethod();

		// public pure virtual functions
		virtual void					loadRequest(const std::string& readLine) = 0;
		virtual bool					checkMethodLimit(const std::vector<std::string>& limitExcept) const = 0;
		virtual bool					isMethodCreateFin(void) = 0;
		virtual void					logMethodInfo(std::fstream& logFile) const = 0;
		virtual void					uriParse(void) = 0;
		virtual void					doMethodWork(void) = 0;
		virtual std::string&			getReadBody(void) = 0;
		virtual void					filePathParse(std::string uri) = 0;

		// puclic member functions
		void							queryStringParse(std::string& uri);
		void							directoryParse(std::string& uri,
													   std::vector<std::string>& dirVec);
		void							launchCgi(void);
		// getter functions
		int								getStatusCode(void) const;
		const std::string&				getMethod(void) const;
		const std::string&				getUri(void) const;
		const int32_t&					getBodySize(void) const;
		// const std::string&				getCgiPath(void) const;
		// const std::string&				getCgiExt(void) const;
		const configInfo&				getConfig(void) const;
		const std::string&				getHttpVersion(void) const;
		const requestMap&				getRequestSet(void) const;
		const std::string&				getFilePath(void) const;
		const std::string&				getQueryString(void) const;
		const std::string&				getFileExt(void) const;
		int								getCrlfCnt(void) const;
		configInfo::Location*			getLocation(void) const;
		const std::string&				getBodyType(void) const;
		bool							getIsCgi(void) const;
};
#endif //AMethod_hpp
