#ifndef configInfo_hpp
#define configInfo_hpp

#include <cstdlib>
#include <cctype>

#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

#include "../WsException.hpp"

class configInfo
{
	public:
		struct Location
		{
			Location(const std::string &path);
			std::vector<std::string>	locIndex;
			std::vector<std::string>	locExpires;
			std::vector<std::string>	locProxyPass;
			std::vector<std::string>	locLimitExpect;
			std::string					locPath;
			std::string					locRoot;
			std::string					locCgiPass;
			std::string					locAlias;
			std::string					locCgiExt;
			std::string					locAutoIndex;
			int32_t						clientMaxBodySize;
		};

	private:
		std::vector<std::string>		m_index;
		std::vector<std::string>		m_serverName;
		std::vector<std::string>		m_errorCode;
		std::string						m_root;
		std::string						m_errorPath;
		int32_t							m_listen;
		int32_t							m_clientMaxBodySize;
		int32_t							m_uriBufferSize;

		std::map<std::string, Location>	m_mapLocation;
		std::vector<Location>			m_location;

		bool	isPath(const std::string& str);
		bool	isPath(const std::vector<std::string>& str);
		bool	isNum(const std::string& str);
		bool	isNum(const std::vector<std::string>& str);
		bool	isMethod(const std::vector<std::string>& method);

	public:
		// constructor & destructor
		configInfo();
		~configInfo();

		// copy constructor & copy operator
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
		void		setUriBufferSize(std::vector<std::string>& set);
		void		setClientMaxBodySize(std::vector<std::string>& set);
		void		setErrorPage(std::vector<std::string>& set);
		void		setLocationExpires(std::vector<std::string>& set);
		void		setLocationRoot(std::vector<std::string>& set);
		void		setLocationProxy(std::vector<std::string>& set);
		void		setLocationCgiPass(std::vector<std::string>& set);
		void		setLocationLimitExcept(std::vector<std::string>& set);
		void		setLocationIndex(std::vector<std::string>& set);
		void		setLocationAlias(std::vector<std::string>& set);
		void		setLocationClientMaxBodySize(std::vector<std::string>& set);
		void		setLocationCgiExt(std::vector<std::string>& set);
		void		setLocationAutoIdx(std::vector<std::string>& set);


		// getter
		int32_t							getListenPort(void) const;
		std::string						getRootPath(void) const;
		std::vector<std::string>		getIndexFile(void) const;
		std::vector<std::string>		getServerName(void) const;
		int32_t							getUriBufferSize(void) const;
		int32_t							getClinetMaxBodySize(void) const;
		std::vector<Location>			getLocation(void) const;
		std::vector<std::string>		getErrorCode(void) const;
		std::string						getErrorPath(void) const;
		std::map<std::string, Location>	getMapLocation(void) const;


		int			createLocation(std::string& path);
		void		createDefaultLocation(void);
		void		checkConfig(void);
		void		printServerBlock(std::ostream& os) const;
		void		printLocationBlock(std::ostream& os, size_t i) const;
		Location*	findLocation(const std::string& locationPath);
		int			isLocationBlock(const std::vector<std::string>& directoryVec);
		void		locationVecToMap(void);

		friend std::ostream&			operator<<(std::ostream &os, const configInfo& conf);
};

#endif //configInfo_hpp
