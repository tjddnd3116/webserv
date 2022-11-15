#ifndef AMethod_hpp
#define AMethod_hpp

#include <fstream>
#include <sys/stat.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../parser/configInfo.hpp"

#define RESET	"\033[0m"
#define RED		"\033[31m"
#define BLUE    "\033[34m"

class AMethod
{
	typedef std::map<std::string, std::vector<std::string> > requestMap;

	protected:
		std::string						m_methodType;
		std::string						m_uri;
		std::string						m_httpVersion;
		std::string						s;
		// std::string						m_body;
		requestMap						m_requestSet;
		configInfo						m_conf;
		int								m_statusCode;

		std::string						m_filePath;
		std::string						m_queryString;
		std::string						m_fileExt;
		int								m_crlfCnt;
		configInfo::Location*			m_location;

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
		virtual const std::string&		getReadBody(void) const = 0;
		virtual void					filePathParse(std::string uri) = 0;

		// public virtual functions
		virtual const std::string&		getBody(void) const;

		// puclic member functions
		void							queryStringParse(std::string& uri);
		void							directoryParse(std::string& uri,
													   std::vector<std::string>& dirVec);
		// getter functions
		int								getStatusCode(void) const;
		const std::string&				getMethod(void) const;
		const std::string&				getUri(void) const;
		const configInfo&				getConfig(void) const;
		const std::string&				getHttpVersion(void) const;
		const requestMap&				getRequestSet(void) const;
		const std::string&				getFilePath(void) const;
		const std::string&				getQueryString(void) const;
		const std::string&				getFileExt(void) const;
		int								getCrlfCnt(void) const;
};
#endif //AMethod_hpp
