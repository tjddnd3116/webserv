#ifndef WsGetMethod_hpp
#define WsGetMethod_hpp

#include "WsIMethod.hpp"

class WsGetMethod : public WsIMethod
{
	public:
		WsGetMethod(const std::string& readLine);
		~WsGetMethod();

		virtual void 	loadRequest(const std::string &readLine);
		virtual void	printBody(void) const;

};
#endif //WsGetMethod_hpp
