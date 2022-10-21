#include "configInfo.hpp"

std::unordered_map<std::string, configInfo::t_setterType>	configInfo::s_table;

configInfo::limitExpect::limitExpect(std::vector<std::string>& method)
{
	limitMethod = method;
}

configInfo::Location::Location(std::string &path)
{
	locPath = path;
}

configInfo::configInfo()
{
	m_serverName.push_back("");
	m_clientMaxBodySize = 1000000;
	m_root.push_back("html");
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
	m_errorPage = copy.getErrorPage();
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
	s_table["client_max_body_size"] = &configInfo::setClientMaxBodySize;
	s_table["error_page"] = &configInfo::setErrorPage;
	s_table["limit_except"] = &configInfo::setLimitExcept;
	s_table["limit_allow"] = &configInfo::setLimitAllow;
	s_table["limit_deny"] = &configInfo::setLimitDeny;
	s_table["client_header_buffer_size"] = &configInfo::setUriBufferSize;
}

void
configInfo::setRootPath(std::vector<std::string>& set)
{
	for (size_t i = 0; i < set.size(); i++)
		if (!isPath(set))
			throw WsException();
	m_root = set;
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
	for (size_t i = 0; i < set.size(); i++)
	{
		if (!isNum(set))
			throw WsException();
		m_listen.push_back(atoi(set[i].c_str()));
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

void	 configInfo::setClientMaxBodySize(std::vector<std::string>& set)
{
	if (set.size() > 1)
	{
		m_clientMaxBodySize = -1;
		return ;
	}
	m_clientMaxBodySize = std::atoi(set[0].c_str()) * 1000000;
}

void	 configInfo::setErrorPage(std::vector<std::string>& set)
{
	std::cout << set[0] << std::endl;
	m_errorPage = set;
}

void	 configInfo::setLimitExcept(std::vector<std::string>& set)
{
	limitExpect tmpLimit(set);

	m_location.back().m_limitExpect.push_back(tmpLimit);
}

void	 configInfo::setLimitAllow(std::vector<std::string>& set)
{
	m_location.back().m_limitExpect.back().limitAllow = set[0];
}

void	 configInfo::setLimitDeny(std::vector<std::string>& set)
{
	m_location.back().m_limitExpect.back().limitDeny = set[0];
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

int
configInfo::createLimitExcept(std::vector<std::string>& method)
{
	if (!isMethod(method))
		return (1);
	limitExpect tmpLimitExcept(method);
	m_location.back().m_limitExpect.push_back(tmpLimitExcept);
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
	if (m_listen.empty())
		throw WsException("listen is emtpy");
	if (!isPath(m_root))
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

bool
configInfo::isMethod(const std::vector<std::string>& method)
{
	for (size_t i = 0; i < method.size(); i++)
	{
		if (method[i] != "GET" && method[i] != "DELETE" && method[i] != "POST")
			return (false);
	}
	return (true);
}

std::vector<int>
configInfo::getListenPort(void) const
{
	return (m_listen);
}

std::vector<std::string>
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

std::vector<std::string>
configInfo::getUriBufferSize(void) const
{
	return (m_uriBufferSize);
}

size_t
configInfo::getClinetMaxBodySize(void) const
{
	return (m_clientMaxBodySize);
}

std::vector<std::string>
configInfo::getErrorPage(void) const
{
	return (m_errorPage);
}

std::ostream&
operator<<(std::ostream &os, const configInfo& conf)
{
	os << "------configInfo-----" << std::endl;
	os << "root : " << std::endl;
	for (size_t i = 0; i < conf.m_root.size(); i++)
		os << "\t" << conf.m_root[i] << std::endl;
	os << "index : " << std::endl;
	for (size_t i = 0; i < conf.m_index.size(); i++)
		os << "\t" << conf.m_index[i] << std::endl;
	os << "server_name : " << std::endl;
	for (size_t i = 0; i < conf.m_serverName.size(); i++)
		os << "\t" << conf.m_serverName[i] << std::endl;
	os << "listen : " << std::endl;
	for (size_t i = 0; i < conf.m_listen.size(); i++)
		os << "\t" << conf.m_listen[i] << std::endl;
	os << "uri buffer size : " << std::endl;
		for (size_t i = 0; i < conf.m_uriBufferSize.size(); i++)
			os << "\t" << conf.m_uriBufferSize[i] << std::endl;

	os << "client_max_body_size : " << std::endl;
		os << "\t" << conf.m_clientMaxBodySize << std::endl;

	os << "error_page : " << std::endl;
		for (size_t i = 0; i < conf.m_errorPage.size(); i++)
			os << "\t" << conf.m_errorPage[i] << std::endl;

	for (size_t i = 0; i < conf.m_location.size(); i++)
	{
		os << "location { " << std::endl;
		os << "\tpath : " << std::endl;
		os << "\t\t" << conf.m_location[i].locPath << std::endl;
		os << "\troot :" << std::endl;
		for (size_t j = 0; j < conf.m_location[i].locRoot.size(); j++)
			os << "\t\t" << conf.m_location[i].locRoot[j] << std::endl;

		os << "\texpries :" << std::endl;
		for (size_t j = 0; j < conf.m_location[i].locExpires.size(); j++)
			os << "\t\t" << conf.m_location[i].locExpires[j] << std::endl;

		os << "\tpass :" << std::endl;
		for (size_t j = 0; j < conf.m_location[i].locProxyPass.size(); j++)
			os << "\t\t" << conf.m_location[i].locProxyPass[j] << std::endl;

		if (!conf.m_location[i].m_limitExpect.empty())
		{
			os << "\tlimit_except : ";
			for (size_t j = 0; j < conf.m_location[i].m_limitExpect[0].limitMethod.size(); j++)
				os << conf.m_location[i].m_limitExpect[0].limitMethod[j] << " ";
			os << "{" << std::endl;
			os << "\tallow : " << std::endl;;
			os << "\t\t" << conf.m_location[i].m_limitExpect[0].limitAllow << std::endl;
			os << "\tdeny : " << std::endl;;
			os << "\t\t" << conf.m_location[i].m_limitExpect[0].limitDeny << std::endl;
			os << "\t\t}" << std::endl;
		}

		if (i == conf.m_location.size() - 1)
			os << "\t}" << std::endl;
	}
	os << "---------------------" << std::endl;
	return (os);
}


