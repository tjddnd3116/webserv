#ifndef request_hpp
#define request_hpp

#include <iostream>
#include <string>
#include <vector>

#include "../method/WsGetMethod.hpp"
#include "../method/WsPostMethod.hpp"
#include "../method/WsPutMethod.hpp"
#include "../method/WsDeleteMethod.hpp"
#include "../parser/configInfo.hpp"

class request
{
		private:
			AMethod*		m_method;
			configInfo	m_conf;

		public:
			request(const configInfo& conf);
			~request();

			AMethod*			readRequest(const std::string& request);
			AMethod*			methodGenerator(const std::string& readLine);
};
#endif //request_hpp
