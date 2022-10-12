#include "WsRequest.hpp"

WsRequest::WsRequest()
{
	m_method = NULL;
}

WsRequest::~WsRequest()
{}

void
WsRequest::readRequest(const std::string& request)
{
	size_t		prePos;
	size_t		curPos;

	prePos = 0;
	m_method = NULL;
	curPos = request.find("\n", prePos);
	while (curPos != std::string::npos)
	{
		std::string readLine;

		readLine = request.substr(prePos, curPos - prePos);
		prePos = curPos + 1;
		if (m_method == NULL)
			m_method = methodGenerator(readLine);
		else
			m_method->loadRequest(readLine);
		curPos = request.find("\n", prePos);
	}
}

WsIMethod*
WsRequest::methodGenerator(const std::string& readLine)
{
	std::string method;

	method.assign(readLine, 0, readLine.find(" ", 0));
	if (method == "GET")
		return (new WsGetMethod(readLine));
	else if (method == "POST")
		return (new WsPostMethod(readLine));
	else if (method == "PUT")
		return (new WsPutMethod(readLine));
	return (NULL);
}

const WsIMethod*
WsRequest::getMethod(void) const
{
	return (m_method);
}

void	 WsRequest::printRequest(void) const
{
	m_method->printInfo();
}
