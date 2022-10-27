#ifndef WsException_hpp
#define WsException_hpp

#include <exception>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define NC "\033[0m"
#define RED "\033[31m"

class WsException : public std::exception
{
		private:
			std::string m_errMsg;
			size_t		m_errLine;

		public:
			WsException(const std::string& errMsg = "");
			WsException(size_t lineNum, const std::string& errMsg);

			~WsException() throw();

			const char *what() const throw();
			void		printConfigErr(const std::vector<std::string>& configFile,
									   std::fstream& logFile);
			const std::string& getErrorMsg(void) const;
};
#endif //WsException_hpp
