#ifndef WsPostMethod_hpp
#define WsPostMethod_hpp

#include "WsIMethod.hpp"

class WsPostMethod : public WsIMethod
{
	public:
		WsPostMethod(const std::string& method);
		~WsPostMethod();
};
#endif //WsPostMethod_hpp
