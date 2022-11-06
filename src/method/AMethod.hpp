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
	protected:
		std::string						m_methodType;
		std::string						m_uri;
		std::string						m_httpVersion;
		std::map<std::string, std::vector<std::string> >
										m_requestSet;
		int								m_statusCode;
		configInfo						m_conf;
		std::string						s;
		std::string						m_body;

		std::vector<std::string>		m_limitExcept;
		std::string						m_filePath;
		std::string						m_queryString;
		std::string						m_fileExt;
		int								m_crlfCnt;

		std::vector<std::string>	splitReadLine(const std::string& readLine,
												  const std::string& str = " ");
		int							checkStartLine(std::vector<std::string>& splittedLine);
		void						loadBody(const std::string& readLine);
		bool						checkFileExists(const std::string& filePath);
		bool						checkDirExists(const std::string& filePath);
		int							hexToDecimal(const std::string& readLine);
		void						extractExt(std::string& fileName);
		void						filePathParse(std::string uri);
		void						putFilePathParse(std::string uri);
		void						postFilePathParse(std::string uri);
		bool						getTrailingSlash(const std::string& uri);
		void						readFile(std::string& readBody);
		void						writeFile(std::string& bodyBuffer);

	public:
		// constructor & destructor
		AMethod(const std::string& readLine, const configInfo& conf);
		virtual ~AMethod();

		// pure virtual functions
		virtual void					loadRequest(const std::string& readLine) = 0;
		virtual bool					checkMethodLimit(const std::vector<std::string>& limitExcept) const = 0;
		virtual bool					isMethodCreateFin(void) const = 0;
		virtual void					logMethodInfo(std::fstream& logFile) const = 0;
		virtual void					uriParse(void) = 0;
		virtual void					doMethodWork(void) = 0;
		virtual const std::string&		getReadBody(void) const = 0;

		// virtual functions
		virtual const std::string&		getBody(void) const;

		// member functions
		void							queryStringParse(std::string& uri);
		void							directoryParse(std::string& uri,
													   std::vector<std::string>& dirVec);

		// getter
		int								getStatusCode(void) const;
		const std::string&				getMethod(void) const;
		const std::string&				getUri(void) const;
		const configInfo&				getConfig(void) const;
		const std::string&				getHttpVersion(void) const;
		const std::map<std::string, std::vector<std::string> >&
										getRequestSet(void) const;
		const std::string&				getFilePath(void) const;
		const std::string&				getQueryString(void) const;
		const std::string&				getFileExt(void) const;
};
#endif //AMethod_hpp
