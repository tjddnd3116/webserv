#ifndef WsPutMethod_hpp
#define WsPutMethod_hpp

#include "WsIMethod.hpp"

class WsPutMethod : public WsIMethod
{
	public:
		WsPutMethod(const std::string& readLine);
		~WsPutMethod();

};
#endif //WsPutMethod_hpp
