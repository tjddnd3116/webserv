#ifndef WsConfigInfo_hpp
#define WsConfigInfo_hpp

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <cctype>

#include "../WsException.hpp"

class WsConfigInfo
{
	public:
		struct Location
		{
			Location(std::string &path);
			std::string					locPath;
			std::vector<std::string>	locRoot;
			std::vector<std::string>	locExpires;
			std::vector<std::string>	locProxyPass;
		};

	private:
		std::vector<std::string>	m_rootPath;
		std::vector<std::string>	m_indexFile;
		std::vector<std::string>	m_serverName;
		std::vector<int>			m_listenPort;
		std::vector<Location>		m_location;

		// just printing info tur / false
		// after use delete it
		const static bool						print_flag = false;

		static bool		isPath(const std::string& str);
		static bool		isPath(const std::vector<std::string>& str);
		static bool		isNum(const std::string& str);
		static bool		isNum(const std::vector<std::string>& str);

	public:
		WsConfigInfo();
		~WsConfigInfo();
		WsConfigInfo(const WsConfigInfo& copy);
		WsConfigInfo& operator=(const WsConfigInfo& copy);


		typedef void 		(t_setter)(std::vector<std::string>&);
		typedef t_setter	WsConfigInfo::*t_setterType;
		static std::unordered_map<std::string, t_setterType>	s_table;

		static void			setTable();
		void				setRootPath(std::vector<std::string>& set);
		void				setIndexFile(std::vector<std::string>& set);
		void				setServerName(std::vector<std::string>& set);
		void				setListenPort(std::vector<std::string>& set);
		void				setLocationExpires(std::vector<std::string>& set);
		void				setLocationRoot(std::vector<std::string>& set);
		void				setLocationProxy(std::vector<std::string>& set);

		int					createLocation(std::string& path);
		void				printConf(void) const;
		void				checkConfig(void);

		std::vector<int>			getListenPort(void) const;
		std::vector<std::string>	getRootPath(void) const;
		std::vector<std::string>	getIndexFile(void) const;
		std::vector<std::string>	getServerName(void) const;
		std::vector<Location>		getLocation(void) const;

};

#endif //WsConfigInfo_hpp
