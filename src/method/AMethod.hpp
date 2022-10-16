#ifndef AMethod_hpp
#define AMethod_hpp

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

		std::string		m_method;
		std::string		m_uri;
		std::string		m_httpVersion;
		std::map<std::string, std::vector<std::string> >
						m_requestSet;
		int				m_statusCode;
		configInfo		m_conf;

		std::vector<std::string>	splitReadLine(const std::string& readLine,
				const std::string& str = " ");
		int							checkStartLine(std::vector<std::string>& splittedLine);
		void						loadBody(const std::string& readLine);

	public:
		AMethod(const std::string& readLine, const configInfo& conf);
		virtual ~AMethod();

		virtual void		loadRequest(const std::string& readLine) = 0;
		virtual void		printBody(void) const = 0;

		void				printInfo(void) const;
		const std::string&	getMethod(void) const;
		const std::string&	getUri(void) const;
		const std::string&	getHttpVersion(void) const;
		const std::map<std::string, std::vector<std::string> >&
							getRequestSet(void) const;

		friend std::ostream& operator<<(std::ostream& os, const AMethod& method);

};
#endif //AMethod_hpp
