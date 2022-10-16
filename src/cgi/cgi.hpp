#ifndef cgi_hpp
#define cgi_hpp

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "../method/AMethod.hpp"

class cgi
{
		private:
			std::vector<std::string>	env;
			std::vector<char*>	cenv;
			std::string	cgi_path;
		public:
			cgi();
			~cgi();
			void initCgi(const AMethod *method);
			void execCgi(const AMethod *method);
};
#endif //cgi_hpp
