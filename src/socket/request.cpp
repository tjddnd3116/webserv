#include "request.hpp"
#include <unistd.h>

request::request(const configInfo& conf)
{
	m_conf = conf;
	m_method = NULL;
	m_buffer.clear();
}

request::~request()
{}

int
request::readRequest(const std::string& request)
{
	std::string readLine;
	size_t		prePos;
	size_t		curPos;

	prePos = 0;
	m_buffer += request;
	curPos = m_buffer.find("\n", prePos);
	while (curPos != std::string::npos)
	{
		readLine = m_buffer.substr(prePos, curPos - prePos);
		if (readLine.size() > 1 && readLine.back() == '\r')
			readLine.pop_back();
		prePos = curPos + 1;
		if (m_method == NULL)
			m_method = methodGenerator(readLine);
		else
			m_method->loadRequest(readLine);
		curPos = m_buffer.find("\n", prePos);
	}
	readLine = m_buffer.substr(prePos, m_buffer.size() - prePos);
	m_buffer.clear();
	if (m_method && m_method->getCrlfCnt() == 1 && m_method->getBodyType() != "chunked")
		m_method->loadRequest(readLine);
	else
		m_buffer += readLine;
	if (m_method && m_method->isMethodCreateFin())
		return (READ_FIN);
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
request::setMethodToNull(void)
{
	m_method = NULL;
	m_buffer.clear();
}
