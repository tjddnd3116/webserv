#ifndef WsPutMethod_hpp
#define WsPutMethod_hpp

#include "WsIMethod.hpp"

class WsPutMethod : public WsIMethod
{
	public:
		WsPutMethod(const std::string& readLine, const WsConfigInfo& conf);
		virtual ~WsPutMethod();

		virtual void	loadRequest(const std::string &readLine);
		virtual void	printBody(void) const;
};
#endif //WsPutMethod_hpp
