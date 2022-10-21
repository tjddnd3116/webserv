#ifndef configInfo_hpp
#define configInfo_hpp

#include <cstdlib>
#include <cctype>

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

#include "../WsException.hpp"

class configInfo
{
	private:
		struct limitExpect
		{
			limitExpect(std::vector<std::string>& method);
			std::vector<std::string>	limitMethod;
			std::string					limitAllow;
			std::string					limitDeny;
		};

		struct Location
		{
			Location(std::string &path);
			std::string					locPath;
			std::vector<std::string>	locRoot;
			std::vector<std::string>	locExpires;
			std::vector<std::string>	locProxyPass;
			std::vector<limitExpect>	m_limitExpect;
		};

		std::vector<std::string>	m_root;
		std::vector<std::string>	m_index;
		std::vector<std::string>	m_serverName;
		std::vector<int>			m_listen;
		std::vector<Location>		m_location;
		int32_t						m_clientMaxBodySize;
		std::vector<std::string>	m_errorPage;
		std::vector<std::string>	m_uriBufferSize;

		bool	isPath(const std::string& str);
		bool	isPath(const std::vector<std::string>& str);
		bool	isNum(const std::string& str);
		bool	isNum(const std::vector<std::string>& str);
		bool	isMethod(const std::vector<std::string>& method);

	public:
		configInfo();
		~configInfo();
		configInfo(const configInfo& copy);
		configInfo& operator=(const configInfo& copy);


		typedef void 		(t_setter)(std::vector<std::string>&);
		typedef t_setter	configInfo::*t_setterType;
		static std::unordered_map<std::string, t_setterType>	s_table;

		// setter
		static void	setTable();
		void		setRootPath(std::vector<std::string>& set);
		void		setIndexFile(std::vector<std::string>& set);
		void		setServerName(std::vector<std::string>& set);
		void		setListenPort(std::vector<std::string>& set);
		void		setLocationExpires(std::vector<std::string>& set);
		void		setLocationRoot(std::vector<std::string>& set);
		void		setLocationProxy(std::vector<std::string>& set);
		void		setUriBufferSize(std::vector<std::string>& set);

		void		setClientMaxBodySize(std::vector<std::string>& set);
		void		setErrorPage(std::vector<std::string>& set);
		void		setLimitExcept(std::vector<std::string>& set);
		void		setLimitAllow(std::vector<std::string>& set);
		void		setLimitDeny(std::vector<std::string>& set);

		int			createLocation(std::string& path);
		int			createLimitExcept(std::vector<std::string>& method);
		void		checkConfig(void);

		// getter
		std::vector<int>			getListenPort(void) const;
		std::vector<std::string>	getRootPath(void) const;
		std::vector<std::string>	getIndexFile(void) const;
		std::vector<std::string>	getServerName(void) const;
		std::vector<std::string>	getUriBufferSize(void) const;
		std::vector<Location>		getLocation(void) const;
		size_t						getClinetMaxBodySize(void) const;
		std::vector<std::string>	getErrorPage(void) const;

		friend std::ostream&		operator<<(std::ostream &os, const configInfo& conf);
};

#endif //configInfo_hpp
