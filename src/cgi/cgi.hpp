#ifndef cgi_hpp
#define cgi_hpp

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "../method/AMethod.hpp"

#define READ 0
#define WRITE 1

class cgi
{
	private:
		std::vector<std::string>	m_env;
		std::vector<char*>			m_envChar;
		std::string					m_cgiPath;
		std::string 				m_path;
		std::string					m_script;
		std::string					m_query;

	public:
		cgi();
		~cgi();
		void initCgi(const AMethod *method);
		std::string execCgi(const std::string& readLine);
};
#endif //cgi_hpp
