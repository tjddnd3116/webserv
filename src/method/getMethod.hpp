#ifndef getMethod_hpp
#define getMethod_hpp

#include "AMethod.hpp"

class getMethod : public AMethod
{
	public:
		getMethod(const std::string& readLine, const configInfo& conf);
		~getMethod();

		virtual void 	loadRequest(const std::string &readLine);
		virtual bool	checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool	isMethodCreateFin(void) const;
		virtual void	logMethodInfo(std::fstream& logFile) const;
		virtual void	uriParse(void);

};
#endif //getMethod_hpp
