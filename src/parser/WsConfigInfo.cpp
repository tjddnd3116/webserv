#include "WsConfigInfo.hpp"

std::unordered_map<std::string, WsConfigInfo::t_setterType>	WsConfigInfo::s_table;

WsConfigInfo::Location::Location(std::string &path)
{
	locPath = path;
}

WsConfigInfo::WsConfigInfo()
{}

WsConfigInfo::~WsConfigInfo()
{}

WsConfigInfo::WsConfigInfo(const WsConfigInfo& copy)
{
	*this = copy;
}

WsConfigInfo&
WsConfigInfo::operator=(const WsConfigInfo& copy)
{
	m_rootPath = copy.getRootPath();
	m_indexFile = copy.getIndexFile();
	m_serverName = copy.getServerName();
	m_listenPort = copy.getListenPort();
	m_location = copy.getLocation();
	return (*this);
}

// TODO: location
void	WsConfigInfo::setTable()
{
	s_table["root"] = &WsConfigInfo::setRootPath;
	s_table["index"] = &WsConfigInfo::setIndexFile;
	s_table["server_name"] = &WsConfigInfo::setServerName;
	s_table["listen"] = &WsConfigInfo::setListenPort;
	s_table["loc_expires"] = &WsConfigInfo::setLocationExpires;
	s_table["loc_root"] = &WsConfigInfo::setLocationRoot;
	s_table["loc_proxy_pass"] = &WsConfigInfo::setLocationProxy;
}

// TODO: push_back
void	WsConfigInfo::setRootPath(std::vector<std::string>& set)
{
	for (size_t i = 0; i < set.size(); i++)
		if (!isPath(set))
			throw WsException();
	m_rootPath = set;
}

void	WsConfigInfo::setIndexFile(std::vector<std::string>& set)
{
	m_indexFile = set;
}

void	WsConfigInfo::setServerName(std::vector<std::string>& set)
{
	m_serverName = set;
}

void	WsConfigInfo::setListenPort(std::vector<std::string>& set)
{
	for (size_t i = 0; i < set.size(); i++)
	{
		if (!isNum(set))
			throw WsException();
		m_listenPort.push_back(atoi(set[i].c_str()));
	}
}

void	 WsConfigInfo::setLocationExpires(std::vector<std::string>& set)
{
	m_location.back().locExpires = set;
}

void	 WsConfigInfo::setLocationRoot(std::vector<std::string>& set)
{
	m_location.back().locRoot = set;
}

void	 WsConfigInfo::setLocationProxy(std::vector<std::string>& set)
{
	m_location.back().locProxyPass = set;
}

int		WsConfigInfo::createLocation(std::string& path)
{
	if (!isPath(path))
		return (1);
	Location location(path);
	m_location.push_back(location);
	return (0);
}

void WsConfigInfo::printConf(void) const
{
	if (!print_flag)
		return ;
	std::cout << "------configInfo-----" << std::endl;
	std::cout << "root : " << std::endl;
	for (size_t i = 0; i < m_rootPath.size(); i++)
		std::cout << "\t" << m_rootPath[i] << std::endl;
	std::cout << "index : " << std::endl;
	for (size_t i = 0; i < m_indexFile.size(); i++)
		std::cout << "\t" << m_indexFile[i] << std::endl;
	std::cout << "server_name : " << std::endl;
	for (size_t i = 0; i < m_serverName.size(); i++)
		std::cout << "\t" << m_serverName[i] << std::endl;
	std::cout << "listen : " << std::endl;
	for (size_t i = 0; i < m_listenPort.size(); i++)
		std::cout << "\t" << m_listenPort[i] << std::endl;
	for (size_t i = 0; i < m_location.size(); i++)
	{
		std::cout << "location { " << std::endl;
		std::cout << "\tpath : " << m_location[i].locPath << std::endl;
		std::cout << "\troot :" << std::endl;
		for (size_t j = 0; j < m_location[i].locRoot.size(); j++)
			std::cout << "\t\t" << m_location[i].locRoot[j] << std::endl;

		std::cout << "\texpries :" << std::endl;
		for (size_t j = 0; j < m_location[i].locExpires.size(); j++)
			std::cout << "\t\t" << m_location[i].locExpires[j] << std::endl;

		std::cout << "\tpass :" << std::endl;
		for (size_t j = 0; j < m_location[i].locProxyPass.size(); j++)
			std::cout << "\t\t" << m_location[i].locProxyPass[j] << std::endl;
		if (i == m_location.size() - 1)
			std::cout << "\t}" << std::endl;
	}
	std::cout << "---------------------" << std::endl;
}

void	 WsConfigInfo::checkConfig(void)
{
	if (m_indexFile.empty())
		throw WsException("index is emtpy");
	if (m_rootPath.empty())
		throw WsException("root is emtpy");
	if (m_serverName.empty())
		throw WsException("server_name is emtpy");
	if (m_listenPort.empty())
		throw WsException("listen is emtpy");
	if (!isPath(m_rootPath))
		throw WsException("invalid server root path");
}

bool	 WsConfigInfo::isPath(const std::string& str)
{
	if (str.front() != '/')
		return (false);
	return (true);
}

bool	 WsConfigInfo::isPath(const std::vector<std::string>& str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i].front() != '/')
			return (false);
	}
	return (true);
}

bool	 WsConfigInfo::isNum(const std::string& str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			return (false);
	}
	return (true);
}

bool	 WsConfigInfo::isNum(const std::vector<std::string>& str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		for (size_t j = 0; j < str[i].size(); j++)
			if (!isdigit(str[i][j]))
				return (false);
	}
	return (true);
}

std::vector<int>
WsConfigInfo::getListenPort(void) const
{
	return (m_listenPort);
}

std::vector<std::string>
WsConfigInfo::getRootPath(void) const
{
	return (m_rootPath);
}

std::vector<std::string>
WsConfigInfo::getIndexFile(void) const
{
	return (m_indexFile);
}

std::vector<std::string>
WsConfigInfo::getServerName(void) const
{
	return (m_serverName);
}

std::vector<WsConfigInfo::Location>
WsConfigInfo::getLocation(void) const
{
	return (m_location);
}

