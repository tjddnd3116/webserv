#ifndef cgi_hpp
#define cgi_hpp

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "../method/AMethod.hpp"
#include "../method/postMethod.hpp"

#define READ 0
#define WRITE 1

class cgi
{
		private:
			std::vector<std::string>	env;
			std::vector<char*>	cenv;
			std::string	cgi_path;
			std::string path;
    		std::string script;
    		std::string query;
		public:
			cgi();
			~cgi();
			void initCgi(const AMethod *method);
			std::string execCgi(const AMethod *method);
};
#endif //cgi_hpp
