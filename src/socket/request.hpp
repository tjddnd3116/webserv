#ifndef request_hpp
#define request_hpp

#include <iostream>
#include <string>
#include <vector>

#include "../method/getMethod.hpp"
#include "../method/headMethod.hpp"
#include "../method/postMethod.hpp"
#include "../method/putMethod.hpp"
#include "../method/deleteMethod.hpp"
#include "../parser/configInfo.hpp"

// method maker
class request
{
		private:
			AMethod*	m_method;
			configInfo	m_conf;

			request(const request& copy);
			request& operator=(const request& copy);

		public:
			request(const configInfo& conf);
			~request();

			AMethod*			readRequest(const std::string& request);
			AMethod*			methodGenerator(const std::string& readLine);
};
#endif //request_hpp
