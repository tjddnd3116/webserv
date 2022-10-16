#ifndef deleteMethod_hpp
#define deleteMethod_hpp

#include "AMethod.hpp"

class deleteMethod : public AMethod
{
	public:
		deleteMethod(const std::string& readLine, const configInfo& conf);
		~deleteMethod();

		virtual void	loadRequest(const std::string &readLine);
		virtual void	printBody(void) const;

};
#endif //deleteMethod_hpp
