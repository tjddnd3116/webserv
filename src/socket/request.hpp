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

class request
{
		private:
			AMethod*	m_method;
			configInfo	m_conf;
			std::string	m_buffer;

			// hide copy constructor & operator
			request(const request& copy);
			request& operator=(const request& copy);

		public:
			// constructor & destructor
			request(const configInfo& conf);
			~request();

			// public member functions
			AMethod*			methodGenerator(const std::string& readLine);
			int					readRequest(const std::string& request);

			// getter functions
			AMethod*			getMethod(void) const;

			// setter functions
			void				setMethodToNull(void);
};
#endif //request_hpp
