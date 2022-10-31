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

#define READ_FIN 0
#define READING 1


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

			// AMethod*			readRequest(const std::string& request);
			AMethod*			methodGenerator(const std::string& readLine);

			AMethod*			getMethod(void) const;
			int					readRequest(const std::string& request);
			void				clearRequest(void);
};
#endif //request_hpp
