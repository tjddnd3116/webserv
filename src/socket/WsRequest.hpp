#ifndef WsRequest_hpp
#define WsRequest_hpp

#include <iostream>
#include <string>
#include <vector>

#include "../method/WsGetMethod.hpp"
#include "../method/WsPostMethod.hpp"
#include "../method/WsPutMethod.hpp"
#include "../method/WsDeleteMethod.hpp"
#include "../parser/WsConfigInfo.hpp"

class WsRequest
{
		private:
			WsIMethod*		m_method;
			WsConfigInfo	m_conf;

		public:
			WsRequest(const WsConfigInfo& conf);
			~WsRequest();

			WsIMethod*			readRequest(const std::string& request);
			WsIMethod*			methodGenerator(const std::string& readLine);
};
#endif //WsRequest_hpp
