#ifndef WsGetMethod_hpp
#define WsGetMethod_hpp

#include "WsIMethod.hpp"

class WsGetMethod : public WsIMethod
{
	public:
		WsGetMethod(const std::string& readLine);
		~WsGetMethod();

};
#endif //WsGetMethod_hpp
