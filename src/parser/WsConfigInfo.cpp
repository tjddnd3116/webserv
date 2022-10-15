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
	m_uriBufferSize = copy.getUriBufferSize();
	return (*this);
}

void	WsConfigInfo::setTable()
{
	s_table["root"] = &WsConfigInfo::setRootPath;
	s_table["index"] = &WsConfigInfo::setIndexFile;
	s_table["server_name"] = &WsConfigInfo::setServerName;
	s_table["listen"] = &WsConfigInfo::setListenPort;
	s_table["loc_expires"] = &WsConfigInfo::setLocationExpires;
	s_table["loc_root"] = &WsConfigInfo::setLocationRoot;
	s_table["loc_proxy_pass"] = &WsConfigInfo::setLocationProxy;
	s_table["client_header_buffer_size"] = &WsConfigInfo::setUriBufferSize;
}

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

void			 WsConfigInfo::setUriBufferSize(std::vector<std::string>& set)
{
	m_uriBufferSize = set;
}

int		WsConfigInfo::createLocation(std::string& path)
{
	if (!isPath(path))
		return (1);
	Location location(path);
	m_location.push_back(location);
	return (0);
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

std::vector<std::string>
WsConfigInfo::getUriBufferSize(void) const
{
	return (m_uriBufferSize);
}

std::ostream&
operator<<(std::ostream &os, const WsConfigInfo& conf)
{
	os << "------configInfo-----" << std::endl;
	os << "root : " << std::endl;
	for (size_t i = 0; i < conf.m_rootPath.size(); i++)
		os << "\t" << conf.m_rootPath[i] << std::endl;
	os << "index : " << std::endl;
	for (size_t i = 0; i < conf.m_indexFile.size(); i++)
		os << "\t" << conf.m_indexFile[i] << std::endl;
	os << "server_name : " << std::endl;
	for (size_t i = 0; i < conf.m_serverName.size(); i++)
		os << "\t" << conf.m_serverName[i] << std::endl;
	os << "listen : " << std::endl;
	for (size_t i = 0; i < conf.m_listenPort.size(); i++)
		os << "\t" << conf.m_listenPort[i] << std::endl;
	os << "uri buffer size : " << std::endl;
		for (size_t i = 0; i < conf.m_uriBufferSize.size(); i++)
			os << "\t" << conf.m_uriBufferSize[i] << std::endl;

	for (size_t i = 0; i < conf.m_location.size(); i++)
	{
		os << "location { " << std::endl;
		os << "\tpath : " << conf.m_location[i].locPath << std::endl;
		os << "\troot :" << std::endl;
		for (size_t j = 0; j < conf.m_location[i].locRoot.size(); j++)
			os << "\t\t" << conf.m_location[i].locRoot[j] << std::endl;

		os << "\texpries :" << std::endl;
		for (size_t j = 0; j < conf.m_location[i].locExpires.size(); j++)
			os << "\t\t" << conf.m_location[i].locExpires[j] << std::endl;

		os << "\tpass :" << std::endl;
		for (size_t j = 0; j < conf.m_location[i].locProxyPass.size(); j++)
			os << "\t\t" << conf.m_location[i].locProxyPass[j] << std::endl;
		if (i == conf.m_location.size() - 1)
			os << "\t}" << std::endl;
	}
	os << "---------------------" << std::endl;
	return (os);
}

