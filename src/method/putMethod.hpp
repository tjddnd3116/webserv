#ifndef putMethod_hpp
#define putMethod_hpp

#include "AMethod.hpp"

class putMethod : public AMethod
{
	public:
		putMethod(const std::string& readLine, const configInfo& conf);
		~putMethod();

		virtual void	loadRequest(const std::string &readLine);
		virtual void	printBody(void) const;
		virtual bool	checkMethodLimit(const std::vector<std::string>& limitExcept) const;
};
#endif //putMethod_hpp
