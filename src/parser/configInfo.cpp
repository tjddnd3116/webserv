#include "configInfo.hpp"

std::unordered_map<std::string, configInfo::t_setterType>	configInfo::s_table;

configInfo::Location::Location(std::string &path)
{
	locPath = path;
}

configInfo::configInfo()
{}

configInfo::~configInfo()
{}

configInfo::configInfo(const configInfo& copy)
{
	*this = copy;
}

configInfo&
configInfo::operator=(const configInfo& copy)
{
	m_rootPath = copy.getRootPath();
	m_indexFile = copy.getIndexFile();
	m_serverName = copy.getServerName();
	m_listenPort = copy.getListenPort();
	m_location = copy.getLocation();
	m_uriBufferSize = copy.getUriBufferSize();
	return (*this);
}

void
configInfo::setTable()
{
	s_table["root"] = &configInfo::setRootPath;
	s_table["index"] = &configInfo::setIndexFile;
	s_table["server_name"] = &configInfo::setServerName;
	s_table["listen"] = &configInfo::setListenPort;
	s_table["loc_expires"] = &configInfo::setLocationExpires;
	s_table["loc_root"] = &configInfo::setLocationRoot;
	s_table["loc_proxy_pass"] = &configInfo::setLocationProxy;
	s_table["client_header_buffer_size"] = &configInfo::setUriBufferSize;
}

void
configInfo::setRootPath(std::vector<std::string>& set)
{
	for (size_t i = 0; i < set.size(); i++)
		if (!isPath(set))
			throw WsException();
	m_rootPath = set;
}

void
configInfo::setIndexFile(std::vector<std::string>& set)
{
	m_indexFile = set;
}

void
configInfo::setServerName(std::vector<std::string>& set)
{
	m_serverName = set;
}

void
configInfo::setListenPort(std::vector<std::string>& set)
{
	for (size_t i = 0; i < set.size(); i++)
	{
		if (!isNum(set))
			throw WsException();
		m_listenPort.push_back(atoi(set[i].c_str()));
	}
}

void
configInfo::setLocationExpires(std::vector<std::string>& set)
{
	m_location.back().locExpires = set;
}

void
configInfo::setLocationRoot(std::vector<std::string>& set)
{
	m_location.back().locRoot = set;
}

void
configInfo::setLocationProxy(std::vector<std::string>& set)
{
	m_location.back().locProxyPass = set;
}

void
configInfo::setUriBufferSize(std::vector<std::string>& set)
{
	m_uriBufferSize = set;
}

int
configInfo::createLocation(std::string& path)
{
	if (!isPath(path))
		return (1);
	Location location(path);
	m_location.push_back(location);
	return (0);
}

void
configInfo::checkConfig(void)
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
	if (m_uriBufferSize.empty())
		m_uriBufferSize.push_back("3k");
}

bool
configInfo::isPath(const std::string& str)
{
	if (str.front() != '/')
		return (false);
	return (true);
}

bool
configInfo::isPath(const std::vector<std::string>& str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i].front() != '/')
			return (false);
	}
	return (true);
}

bool
configInfo::isNum(const std::string& str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			return (false);
	}
	return (true);
}

bool
configInfo::isNum(const std::vector<std::string>& str)
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
configInfo::getListenPort(void) const
{
	return (m_listenPort);
}

std::vector<std::string>
configInfo::getRootPath(void) const
{
	return (m_rootPath);
}

std::vector<std::string>
configInfo::getIndexFile(void) const
{
	return (m_indexFile);
}

std::vector<std::string>
configInfo::getServerName(void) const
{
	return (m_serverName);
}

std::vector<configInfo::Location>
configInfo::getLocation(void) const
{
	return (m_location);
}

std::vector<std::string>
configInfo::getUriBufferSize(void) const
{
	return (m_uriBufferSize);
}

std::ostream&
operator<<(std::ostream &os, const configInfo& conf)
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

