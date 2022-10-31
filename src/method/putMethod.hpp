#ifndef putMethod_hpp
#define putMethod_hpp

#include "AMethod.hpp"

class putMethod : public AMethod
{
	public:
		putMethod(const std::string& readLine, const configInfo& conf);
		~putMethod();

		virtual void	loadRequest(const std::string &readLine);
		virtual bool	checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool	isMethodCreateFin(void) const;
		virtual void	logMethodInfo(std::fstream& logFile) const;
};
#endif //putMethod_hpp
