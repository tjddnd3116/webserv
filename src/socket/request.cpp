#include "request.hpp"

request::request(const configInfo& conf)
{
	m_conf = conf;
	m_method = NULL;
}

request::~request()
{}

AMethod*
request::readRequest(const std::string& request)
{
	size_t		prePos;
	size_t		curPos;
	std::string readLine;
	std::string buffer;

	prePos = 0;
	curPos = request.find("\n", prePos);
	std::cout << request << std::endl;
	while (curPos != std::string::npos)
	{
		readLine = request.substr(prePos, curPos - prePos);
		if (readLine.size() > 0 && readLine.back() == '\r')
			readLine.pop_back();
		prePos = curPos + 1;
		if (m_method == NULL)
			m_method = methodGenerator(readLine);
		else
			m_method->loadRequest(readLine);
		curPos = request.find("\n", prePos);
	}
	readLine = request.substr(prePos, request.size() - prePos);
	m_method->loadRequest(readLine);
	m_method->uriParse();
	return (m_method);
}

AMethod*
request::methodGenerator(const std::string& readLine)
{
	std::string method;

	method.assign(readLine, 0, readLine.find(" ", 0));
	if (method == "GET")
		return (new getMethod(readLine, m_conf));
	else if (method == "POST")
		return (new postMethod(readLine, m_conf));
	else if (method == "PUT")
		return (new putMethod(readLine, m_conf));
	else if (method == "DELETE")
		return (new deleteMethod(readLine, m_conf));
	// else
	//     thow error;
	return (NULL);
}

