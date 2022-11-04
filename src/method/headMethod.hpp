#ifndef headMethod_hpp
#define headMethod_hpp

#include "AMethod.hpp"

class headMethod : public AMethod
{
	public:
		headMethod(const std::string& readLine, const configInfo& conf);
		~headMethod();

		virtual void 	loadRequest(const std::string &readLine);
		virtual bool	checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool	isMethodCreateFin(void) const;
		virtual void	logMethodInfo(std::fstream& logFile) const;
		virtual void	uriParse(void);
};
#endif //headMethod_hpp
