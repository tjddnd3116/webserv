#ifndef AMethod_hpp
#define AMethod_hpp

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
		std::string						m_method;
		std::string						m_uri;
		std::string						m_httpVersion;
		std::map<std::string, std::vector<std::string> >
										m_requestSet;
		int								m_statusCode;
		configInfo						m_conf;
		std::string						s;

		std::string						m_filePath;
		std::string						m_queryString;
		std::string						m_fileExt;

		std::vector<std::string>	splitReadLine(const std::string& readLine,
												  const std::string& str = " ");

		int							checkStartLine(std::vector<std::string>& splittedLine);
		void						loadBody(const std::string& readLine);
		bool						checkFileExists(const std::string& filePath);

	public:
		// constructor & destructor
		AMethod(const std::string& readLine, const configInfo& conf);
		virtual ~AMethod();

		// pure virtual functions
		virtual void					loadRequest(const std::string& readLine) = 0;
		virtual void					printBody(void) const = 0;
		virtual bool					checkMethodLimit(const std::vector<std::string>& limitExcept) const = 0;

		// virtual functions
		virtual const std::string&		getBody(void) const;

		// member functions
		void							uriParse(void);
		void							printInfo(void) const;

		int								getStatusCode(void) const;
		const std::string&				getMethod(void) const;
		const std::string&				getUri(void) const;
		const configInfo&				getConfig(void) const;
		const std::string&				getHttpVersion(void) const;
		const std::map<std::string, std::vector<std::string> >&
										getRequestSet(void) const;
		const std::string&				getFilePath(void) const;
		const std::string&				getQueryString(void) const;

		// friend functions
		friend std::ostream&			operator<<(std::ostream& os,
												   const AMethod& method);

};
#endif //AMethod_hpp
