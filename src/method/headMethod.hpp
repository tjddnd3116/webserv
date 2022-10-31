#ifndef headMethod_hpp
#define headMethod_hpp

#include "AMethod.hpp"

class headMethod : public AMethod
{
	public:
		headMethod(const std::string& readLine, const configInfo& conf);
		~headMethod();

		virtual void 	loadRequest(const std::string &readLine);
		virtual void	printBody(void) const;
		virtual bool	checkMethodLimit(const std::vector<std::string>& limitExcept) const;

};
#endif //headMethod_hpp
