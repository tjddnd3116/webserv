#include "tokenizer.hpp"
#include "../WsInitializer.hpp"

tokenizer::tokenizer()
{
	m_tokIdx = 0;
}

tokenizer::~tokenizer()
{}

tokenizer::tokenizer(const tokenizer& copy)
{
	*this = copy;
}

tokenizer&
tokenizer::operator=(const tokenizer& copy)
{
	m_tokVec = copy.m_tokVec;
	m_tokIdx = copy.m_tokIdx;
	return (*this);
}

void
tokenizer::pushBackToken(t_token& token)
{
	if (token.str == "" || isComment(token))
		return ;
	token.type = selectTokenType(token.str);
	m_tokVec.push_back(token);
}

e_tokenType
tokenizer::selectTokenType(const std::string& str) const
{
	if (str == "server")
		return (SERVER);
	else if (str == "{")
		return (OPEN_BRACE);
	else if (str == "}")
		return (CLOSE_BRACE);
	else if (str == ";")
		return (SEMICOLON);
	else
		return (OPTION_CONTEXT);
}

void
tokenizer::parseToken(WsInitializer &initializer)
{
	while (m_tokIdx != m_tokVec.size() && m_tokVec[m_tokIdx].type == SERVER)
	{
		WsConfigInfo info;

		serverParse(info);
		info.printConf();
		initializer.pushBack(info);
	}
	if (m_tokIdx != m_tokVec.size())
		throw WsException(m_tokVec[m_tokIdx].lineNum, "invaild server block");
}

void
tokenizer::serverParse(WsConfigInfo &info)
{
	m_tokIdx++;
	if (isOpenBrace())
		serverContextParse(info);
	if (isCloseBrace())
		m_tokIdx++;
}

void
tokenizer::serverContextParse(WsConfigInfo &info)
{
	std::vector<std::string>	tokenSet;
	size_t						optionLineNum;

	m_tokIdx++;
	while (isOptionContext())
	{
		if (m_tokVec[m_tokIdx].str == "location")
		{
			m_tokIdx++;
			locationParse(info);
			continue ;
		}
		optionLineNum = m_tokVec[m_tokIdx].lineNum;
		std::string &tokenStr = m_tokVec[m_tokIdx++].str;
		if (WsConfigInfo::s_table[tokenStr] == 0)
			throw WsException(m_tokVec[m_tokIdx - 1].lineNum, "invaild server option");
		tokenSet.clear();
		while (!isSemicolon(optionLineNum))
			tokenSet.push_back(m_tokVec[m_tokIdx++].str);
		if (m_tokVec[m_tokIdx].type == SEMICOLON)
			m_tokIdx++;
		else if (m_tokVec[m_tokIdx].str.back() == ';')
		{
			m_tokVec[m_tokIdx].str.pop_back();
			tokenSet.push_back(m_tokVec[m_tokIdx++].str);
		}
		try
		{
			(info.*(WsConfigInfo::s_table[tokenStr]))(tokenSet);
		}
		catch (std::exception& e)
		{
			throw WsException(m_tokVec[m_tokIdx - 1].lineNum, "invaild port or path");
		}
	}
}

bool
tokenizer::verifyInfo(WsConfigInfo& info)
{
	info.checkConfig();
	return (true);
}

void
tokenizer::locationContextParse(WsConfigInfo &info)
{
	std::vector<std::string>	tokenSet;
	size_t						optionLineNum;

	m_tokIdx++;
	while (isOptionContext())
	{
		optionLineNum = m_tokVec[m_tokIdx].lineNum;
		std::string &tokenStr = m_tokVec[m_tokIdx++].str;
		if (WsConfigInfo::s_table["loc_" + tokenStr] == 0)
			throw WsException(m_tokVec[m_tokIdx - 1].lineNum, "invaild location option");
		tokenSet.clear();
		while (!isSemicolon(optionLineNum))
			tokenSet.push_back(m_tokVec[m_tokIdx++].str);
		if (m_tokVec[m_tokIdx].type == SEMICOLON)
			m_tokIdx++;
		else if (m_tokVec[m_tokIdx].str.back() == ';')
		{
			m_tokVec[m_tokIdx].str.pop_back();
			tokenSet.push_back(m_tokVec[m_tokIdx++].str);
		}
		(info.*(WsConfigInfo::s_table["loc_" + tokenStr]))(tokenSet);
	}
}

void
tokenizer::locationParse(WsConfigInfo& info)
{
	if (isLocationPath(info))
		m_tokIdx++;
	if (isOpenBrace())
		locationContextParse(info);
	if (isCloseBrace())
		m_tokIdx++;
}

bool
tokenizer::isSafeIdx(void)
{
	if (m_tokIdx >= m_tokVec.size())
	{
		m_tokIdx--;
		return (false);
	}
	return (true);
}

bool
tokenizer::isComment(const t_token& token) const
{
	if (token.line[token.line.find_first_not_of("\t ")] == '#')
		return (true);
	return (false);
}

bool
tokenizer::isOpenBrace()
{
	if (!isSafeIdx())
		throw WsException(m_tokVec[m_tokIdx].lineNum, "miss '{'");
	else if (m_tokVec[m_tokIdx].type != OPEN_BRACE)
		throw WsException(m_tokVec[m_tokIdx - 1].lineNum, "miss '{'");
	return (true);
}

bool
tokenizer::isCloseBrace()
{
	if (!isSafeIdx() || m_tokVec[m_tokIdx].type != CLOSE_BRACE)
		throw WsException(m_tokVec[m_tokIdx].lineNum + 1, "miss '}'");
	return (true);
}

bool
tokenizer::isOptionContext(void)
{
	if (!isSafeIdx())
	{
		m_tokIdx++;
		return (false);
	}
	if (m_tokVec[m_tokIdx].type == CLOSE_BRACE)
		return (false);
	return (true);
}

bool
tokenizer::isSemicolon(size_t optionLineNum)
{
	if (!isSafeIdx())
		throw WsException(m_tokVec[m_tokIdx].lineNum, "miss ';'");
	if (m_tokVec[m_tokIdx].lineNum != optionLineNum)
		throw WsException(optionLineNum, "miss ';'");
	if (m_tokVec[m_tokIdx].str.back() == ';')
		return (true);
	return (false);
}

bool	 tokenizer::isLocationPath(WsConfigInfo& info)
{
	if (!isSafeIdx())
		throw WsException(m_tokVec[m_tokIdx].lineNum, "invalid location path");
	if (info.createLocation(m_tokVec[m_tokIdx].str))
		throw WsException(m_tokVec[m_tokIdx - 1].lineNum, "invalid location path");
	return (true);
}

