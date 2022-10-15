#ifndef WsPostMethod_hpp
#define WsPostMethod_hpp

#include "WsIMethod.hpp"

class WsPostMethod : public WsIMethod
{
	private:
		bool		m_isBody;
		std::string	m_bodyBuffer;

		void	loadBody(const std::string& readLine);

	public:
		WsPostMethod(const std::string& method, const WsConfigInfo& conf);
		~WsPostMethod();

		virtual void	loadRequest(const std::string& readLine);
		void			printBody(void) const;
};
#endif //WsPostMethod_hpp
