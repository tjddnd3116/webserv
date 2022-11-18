#include "configInfo.hpp"
#include "../method/AMethod.hpp"

std::unordered_map<std::string, configInfo::t_setterType>	configInfo::s_table;

configInfo::Location::Location(const std::string &path)
{
	locPath = path;
	locRoot = "html";
	locAutoIndex = "off";
	clientMaxBodySize = -1;
	locCgiExt = "";
	locCgiPass = "";
	locIndex.push_back("index.html");
}

configInfo::configInfo()
{
	m_serverName.push_back("");
	m_clientMaxBodySize = -1;
	m_uriBufferSize = 3000;
	m_root = "html";
	m_listen = 80;
	m_index.push_back("index.html");
}

configInfo::~configInfo()
{}

configInfo::configInfo(const configInfo& copy)
{
	*this = copy;
}

configInfo&
configInfo::operator=(const configInfo& copy)
{
	m_root = copy.getRootPath();
	m_index = copy.getIndexFile();
	m_serverName = copy.getServerName();
	m_listen = copy.getListenPort();
	m_location = copy.getLocation();
	m_uriBufferSize = copy.getUriBufferSize();
	m_clientMaxBodySize = copy.getClinetMaxBodySize();
	m_errorCode = copy.getErrorCode();
	m_errorPath = copy.getErrorPath();
	m_mapLocation = copy.getMapLocation();
	return (*this);
}

void
configInfo::setTable()
{
	s_table["root"] = &configInfo::setRootPath;
	s_table["index"] = &configInfo::setIndexFile;
	s_table["server_name"] = &configInfo::setServerName;
	s_table["listen"] = &configInfo::setListenPort;
	s_table["error_page"] = &configInfo::setErrorPage;
	s_table["client_max_body_size"] = &configInfo::setClientMaxBodySize;
	s_table["client_header_buffer_size"] = &configInfo::setUriBufferSize;
	s_table["loc_expires"] = &configInfo::setLocationExpires;
	s_table["loc_root"] = &configInfo::setLocationRoot;
	s_table["loc_index"] = &configInfo::setLocationIndex;
	s_table["loc_proxy_pass"] = &configInfo::setLocationProxy;
	s_table["loc_limit_except"] = &configInfo::setLocationLimitExcept;
	s_table["loc_cgi_pass"] = &configInfo::setLocationCgiPass;
	s_table["loc_client_max_body_size"] = &configInfo::setLocationClientMaxBodySize;
	s_table["loc_alias"] = &configInfo::setLocationAlias;
	s_table["loc_cgi_ext"] = &configInfo::setLocationCgiExt;
	s_table["loc_autoindex"] = &configInfo::setLocationAutoIdx;
}

void
configInfo::setRootPath(std::vector<std::string>& set)
{
	if (set.size() != 1)
		throw (WsException("invalid root size"));
	m_root = set[0];
}

void
configInfo::setIndexFile(std::vector<std::string>& set)
{
	m_index = set;
}

void
configInfo::setServerName(std::vector<std::string>& set)
{
	m_serverName = set;
}

void
configInfo::setListenPort(std::vector<std::string>& set)
{
	if (set.size() != 1 || !isNum(set))
			throw WsException("invalid port");
	m_listen = atoi(set[0].c_str());
}

void
configInfo::setLocationExpires(std::vector<std::string>& set)
{
	m_location.back().locExpires = set;
}

void
configInfo::setLocationRoot(std::vector<std::string>& set)
{
	if (set.size() != 1)
		throw (WsException("invalid root size"));
	m_location.back().locRoot = set[0];
}

void
configInfo::setLocationProxy(std::vector<std::string>& set)
{
	m_location.back().locProxyPass = set;
}

void
configInfo::setLocationIndex(std::vector<std::string>& set)
{
	m_location.back().locIndex = set;
}

void
configInfo::setUriBufferSize(std::vector<std::string>& set)
{
	if (set.size() > 1)
		throw (WsException("invalid uri buffer size"));
	if (!isNum(set[0]))
		throw (WsException("invalid uri buffer size"));
	m_uriBufferSize = std::atoi(set[0].c_str());
}

void
configInfo::setClientMaxBodySize(std::vector<std::string>& set)
{
	if (set.size() > 1)
		throw (WsException("invalid client max body size"));
	if (!isNum(set[0]))
		throw (WsException("invalid client max body size"));
	m_clientMaxBodySize = std::atoi(set[0].c_str());
}

void
configInfo::setErrorPage(std::vector<std::string>& set)
{
	m_errorCode = set;
	m_errorPath = m_errorCode.back();
	m_errorPath = m_root + m_errorPath;
	m_errorCode.pop_back();
	if (!isNum(m_errorCode))
		throw (WsException("invalid error code"));

}

void
configInfo::setLocationLimitExcept(std::vector<std::string>& set)
{
	if (!isMethod(set))
		throw (WsException("invalid method"));
	m_location.back().locLimitExpect = set;
}

void
configInfo::setLocationCgiPass(std::vector<std::string>& set)
{
	if (set.size() != 1)
		throw (WsException("invalid cgi_pass size"));
	m_location.back().locCgiPass = set[0];
}

void
configInfo::setLocationAlias(std::vector<std::string>& set)
{
	if (set.size() != 1)
		throw (WsException("invalid alias size"));
	m_location.back().locAlias = set[0];
}

void
configInfo::setLocationClientMaxBodySize(std::vector<std::string>& set)
{
	if (set.size() != 1)
		throw (WsException("invalid client Max body size"));
	m_location.back().clientMaxBodySize = std::stoi(set[0]);
}

void
configInfo::setLocationCgiExt(std::vector<std::string>& set)
{
	if (set.size() != 1)
		throw (WsException("invalid cgi extension size"));
	m_location.back().locCgiExt = set[0];
}

void
configInfo::setLocationAutoIdx(std::vector<std::string>& set)
{
	if (set.size() != 1)
		throw (WsException("invalid autoindex"));
	if (set[0] != "on" && set[0] != "off")
		throw (WsException("invalid autoindex"));
	m_location.back().locAutoIndex = set[0];
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
	if (m_index.empty())
		throw WsException("index is emtpy");
	if (m_root.empty())
		throw WsException("root is emtpy");
	if (m_serverName.empty())
		throw WsException("server_name is emtpy");
	if (!isPath(m_root))
		throw WsException("invalid server root path");
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

bool
configInfo::isMethod(const std::vector<std::string>& method)
{
	for (size_t i = 0; i < method.size(); i++)
	{
		if (method[i] != "GET" && method[i] != "DELETE" && method[i] != "POST"
				&& method[i] != "HEAD" && method[i] != "PUT")
			return (false);
	}
	return (true);
}

int32_t
configInfo::getListenPort(void) const
{
	return (m_listen);
}

std::string
configInfo::getRootPath(void) const
{
	return (m_root);
}

std::vector<std::string>
configInfo::getIndexFile(void) const
{
	return (m_index);
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

int32_t
configInfo::getUriBufferSize(void) const
{
	return (m_uriBufferSize);
}

int32_t
configInfo::getClinetMaxBodySize(void) const
{
	return (m_clientMaxBodySize);
}

std::vector<std::string>
configInfo::getErrorCode(void) const
{
	return (m_errorCode);
}

std::string
configInfo::getErrorPath(void) const
{
	return (m_errorPath);
}

std::map<std::string, configInfo::Location>
configInfo::getMapLocation(void) const
{
	return (m_mapLocation);
}

std::ostream&
operator<<(std::ostream &os, const configInfo& conf)
{
	os << "------configInfo-----" << std::endl;
	conf.printServerBlock(os);
	for (size_t i = 0; i < conf.m_location.size(); i++)
	{
		os << std::endl;
		conf.printLocationBlock(os, i);
	}
	os << "---------------------" << std::endl;
	return (os);
}

void
configInfo::printServerBlock(std::ostream& os) const
{
	os << "server block" << std::endl;
	os << "root : " << m_root << std::endl;

	os << "index :";
	for (size_t i = 0; i < m_index.size(); i++)
		os << " " << m_index[i];
	os << std::endl;

	os << "server_name :";
	for (size_t i = 0; i < m_serverName.size(); i++)
		os << " " << m_serverName[i];
	os << std::endl;

	os << "listen : " << m_listen << std::endl;

	os << "uri buffer size : " << m_uriBufferSize << std::endl;

	os << "client_max_body_size : " << m_clientMaxBodySize << std::endl;

	if (!m_errorCode.empty())
	{
		os << "error_code :";
			for (size_t i = 0; i < m_errorCode.size(); i++)
				os << " " << m_errorCode[i];
		os << std::endl;

		os << "error_Path : " << m_errorPath << std::endl;
	}
}

void
configInfo::printLocationBlock(std::ostream& os, size_t i) const
{
	os << "location block[" << i + 1 << "]" << std::endl;

	os << "path : " << m_location[i].locPath << std::endl;

	os << "root : " << m_location[i].locRoot << std::endl;

	if (!m_location[i].locExpires.empty())
	{
		os << "expries :" << std::endl;
		for (size_t j = 0; j < m_location[i].locExpires.size(); j++)
			os << "\t" << m_location[i].locExpires[j] << std::endl;
	}

	if (m_location[i].locCgiPass.size())
	{
		os << "pass :" << m_location[i].locCgiPass << std::endl;
	}

	os << "clientMaxBodySize :" << m_location[i].clientMaxBodySize << std::endl;

	if (!m_location[i].locLimitExpect.empty())
	{
		os << "limit_except :";
		for (size_t j = 0; j < m_location[i].locLimitExpect.size(); j++)
			os << " " << m_location[i].locLimitExpect[j];
		os << std::endl;
	}
}

configInfo::Location*
configInfo::findLocation(const std::string& locationPath)
{
	std::map<std::string, Location>::iterator mapIt;

	mapIt = m_mapLocation.find(locationPath);
	return (&mapIt->second);
}

void
configInfo::createDefaultLocation(void)
{
	Location defaultLocaiton("/");

	defaultLocaiton.locRoot = m_root;
	defaultLocaiton.locIndex = m_index;
	defaultLocaiton.locLimitExpect.push_back("GET");
	// defaultLocaiton.locAutoIndex = "on";
	m_location.push_back(defaultLocaiton);
}

void
configInfo::locationVecToMap(void)
{
	for (size_t i = 0; i < m_location.size(); i++)
		m_mapLocation.insert(std::make_pair(m_location[i].locPath, m_location[i]));
}

int
configInfo::isLocationBlock(const std::vector<std::string>& directoryVec)
{
	for (size_t i = directoryVec.size() - 1; i >= 0; i--)
	{
		if (m_mapLocation.find(directoryVec[i]) != m_mapLocation.end())
			return (i);
	}
	return (-1);
}




