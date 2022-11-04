#ifndef deleteMethod_hpp
#define deleteMethod_hpp

#include "AMethod.hpp"

class deleteMethod : public AMethod
{
	public:
		deleteMethod(const std::string& readLine, const configInfo& conf);
		~deleteMethod();

		virtual void	loadRequest(const std::string &readLine);
		virtual bool	checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool	isMethodCreateFin(void) const;
		virtual void	logMethodInfo(std::fstream& logFile) const;
		virtual void	uriParse(void);

};
#endif //deleteMethod_hpp
