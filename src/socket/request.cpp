#include "request.hpp"

request::request(const configInfo& conf)
{
	m_conf = conf;
	m_method = NULL;
}

request::~request()
{}

// AMethod*
// request::readRequest(const std::string& request)
// {
//     std::string readLine;
//     std::string buffer;
//     size_t		prePos;
//     size_t		curPos;
//
//     prePos = 0;
//     curPos = request.find("\n", prePos);
//     while (curPos != std::string::npos)
//     {
//         readLine = request.substr(prePos, curPos - prePos);
//         if (readLine.size() > 0 && readLine.back() == '\r')
//             readLine.pop_back();
//         prePos = curPos + 1;
//         if (m_method == NULL)
//             m_method = methodGenerator(readLine);
//         else
//             m_method->loadRequest(readLine);
//         curPos = request.find("\n", prePos);
//     }
//     readLine = request.substr(prePos, request.size() - prePos);
//     m_method->loadRequest(readLine);
//     m_method->uriParse();
//     return (m_method);
// }

int
request::readRequest(const std::string& request)
{
	std::string readLine;
	size_t		prePos;
	size_t		curPos;

	prePos = 0;
	curPos = request.find("\n", prePos);
	while (curPos != std::string::npos)
	{
		readLine = request.substr(prePos, curPos - prePos);
		if (readLine.size() > 1 && readLine.back() == '\r')
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
	if (m_method && m_method->isMethodCreateFin())
	{
		m_method->uriParse();
		return (READ_FIN);
	}
	return (READING);
}

AMethod*
request::methodGenerator(const std::string& readLine)
{
	std::string method;

	method.assign(readLine, 0, readLine.find(" ", 0));
	if (method == "GET")
		return (new class getMethod(readLine, m_conf));
	else if (method == "HEAD")
		return (new headMethod(readLine, m_conf));
	else if (method == "POST")
		return (new postMethod(readLine, m_conf));
	else if (method == "PUT")
		return (new putMethod(readLine, m_conf));
	else if (method == "DELETE")
		return (new deleteMethod(readLine, m_conf));
	return (NULL);
}

AMethod*
request::getMethod(void) const
{
	return (m_method);
}

void
request::setMethodNull(void)
{
	m_method = NULL;
}

