#ifndef getMethod_hpp
#define getMethod_hpp

#include "AMethod.hpp"

class getMethod : public AMethod
{
	public:
		getMethod(const std::string& readLine, const configInfo& conf);
		~getMethod();

		virtual void 	loadRequest(const std::string &readLine);
		virtual void	printBody(void) const;
		virtual bool	checkMethodLimit(const std::vector<std::string>& limitExcept) const;

};
#endif //getMethod_hpp
