#ifndef WsDeleteMethod_hpp
#define WsDeleteMethod_hpp

#include "WsIMethod.hpp"

class WsDeleteMethod : public WsIMethod
{
	public:
		WsDeleteMethod(const std::string& readLine);
		virtual ~WsDeleteMethod();

		virtual void	loadRequest(const std::string &readLine);
		virtual void	printBody(void) const;

};
#endif //WsDeleteMethod_hpp
