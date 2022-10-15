#include "WsRequest.hpp"

WsRequest::WsRequest(const WsConfigInfo& conf)
{
	m_conf = conf;
	m_method = NULL;
}

WsRequest::~WsRequest()
{}

WsIMethod*
WsRequest::readRequest(const std::string& request)
{
	size_t		prePos;
	size_t		curPos;
	std::string readLine;

	prePos = 0;
	curPos = request.find("\n", prePos);
	while (curPos != std::string::npos)
	{
		readLine = request.substr(prePos, curPos - prePos);
		prePos = curPos + 1;
		if (m_method == NULL)
			m_method = methodGenerator(readLine);
		else
			m_method->loadRequest(readLine);
		curPos = request.find("\n", prePos);
	}
	readLine = request.substr(prePos, request.size() - prePos);
	m_method->loadRequest(readLine);
	return (m_method);
}

WsIMethod*
WsRequest::methodGenerator(const std::string& readLine)
{
	std::string method;

	method.assign(readLine, 0, readLine.find(" ", 0));
	if (method == "GET")
		return (new WsGetMethod(readLine, m_conf));
	else if (method == "POST")
		return (new WsPostMethod(readLine, m_conf));
	else if (method == "PUT")
		return (new WsPutMethod(readLine, m_conf));
	// else if (method == "DELETE")
	//     return (new WsDeleteMethod(readLine));
	// else
	//     thow error;
	return (NULL);
}

