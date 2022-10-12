#ifndef WsIMethod_hpp
#define WsIMethod_hpp

#include <iostream>
#include <string>
#include <vector>
#include <map>

#define RESET	"\033[0m"
#define RED		"\033[31m"
#define BLUE    "\033[34m"

class WsIMethod
{
	protected:
		std::string m_method;
		std::string m_uri;
		std::string m_httpVersion;

		std::map<std::string, std::vector<std::string> > m_requestSet;
		std::vector<std::string> splitReadLine(const std::string& readLine,
				const std::string& str = " ");

	public:
		WsIMethod(const std::string& readLine);
		virtual ~WsIMethod();

		void				loadRequest(const std::string& readLine);
		void				printInfo(void) const;
		const std::string&	getUri(void) const;
		const std::string&getHttpVersion(void) const;
		const std::map<std::string, std::vector<std::string> >&
							getRequestSet(void) const;

};
#endif //WsIMethod_hpp
