#include "tokenizer.hpp"

tokenizer::tokenizer()
{
	m_tokVec.clear();
	m_tokIdx = 0;
}

tokenizer::~tokenizer()
{}

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
tokenizer::parseToken(std::vector<configInfo>& config)
{
	while (m_tokIdx != m_tokVec.size() && m_tokVec[m_tokIdx].type == SERVER)
	{
		configInfo info;

		serverParse(info);
		config.push_back(info);
	}
	if (m_tokIdx != m_tokVec.size())
		throw WsException(m_tokVec[m_tokIdx].lineNum, "invaild server block");
}

void
tokenizer::serverParse(configInfo &info)
{
	m_tokIdx++;
	if (isOpenBrace())
		serverContextParse(info);
	if (isCloseBrace())
		m_tokIdx++;
}

void
tokenizer::serverContextParse(configInfo &info)
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
		if (configInfo::s_table[tokenStr] == 0)
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
			(info.*(configInfo::s_table[tokenStr]))(tokenSet);
		}
		catch (std::exception& e)
		{
			throw WsException(m_tokVec[m_tokIdx - 1].lineNum, "invaild port or path");
		}
	}
}

void
tokenizer::locationContextParse(configInfo &info)
{
	std::vector<std::string>	tokenSet;
	size_t						optionLineNum;

	m_tokIdx++;
	while (isOptionContext())
	{
		if (m_tokVec[m_tokIdx].str == "limit_except")
		{
			m_tokIdx++;
			limitExceptParse(info);
			continue;
		}
		optionLineNum = m_tokVec[m_tokIdx].lineNum;
		std::string &tokenStr = m_tokVec[m_tokIdx++].str;
		if (configInfo::s_table["loc_" + tokenStr] == 0)
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
		(info.*(configInfo::s_table["loc_" + tokenStr]))(tokenSet);
	}
}

void
tokenizer::locationParse(configInfo& info)
{
	if (isLocationPath(info))
		m_tokIdx++;
	if (isOpenBrace())
		locationContextParse(info);
	if (isCloseBrace())
		m_tokIdx++;
}

void
tokenizer::limitExceptContextParse(configInfo& info)
{
	std::vector<std::string>	tokenSet;
	size_t						optionLineNum;

	m_tokIdx++;
	while (isOptionContext())
	{
		optionLineNum = m_tokVec[m_tokIdx].lineNum;
		std::string &tokenStr = m_tokVec[m_tokIdx++].str;
		if (configInfo::s_table["limit_" + tokenStr] == 0)
			throw WsException(m_tokVec[m_tokIdx - 1].lineNum, "invaild limit except option");
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
		(info.*(configInfo::s_table["limit_" + tokenStr]))(tokenSet);
	}
}

void
tokenizer::limitExceptParse(configInfo& info)
{
	isLimitExcept(info);
	if (isOpenBrace())
		limitExceptContextParse(info);
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

bool
tokenizer::isLocationPath(configInfo& info)
{
	if (!isSafeIdx())
		throw WsException(m_tokVec[m_tokIdx].lineNum, "invalid location path");
	if (info.createLocation(m_tokVec[m_tokIdx].str))
		throw WsException(m_tokVec[m_tokIdx - 1].lineNum, "invalid location path");
	return (true);
}

bool
tokenizer::isLimitExcept(configInfo& info)
{
	if (!isSafeIdx())
		throw WsException(m_tokVec[m_tokIdx].lineNum, "invalid limit except");
	size_t curLineNum;
	std::vector<std::string> tokSet;
	curLineNum = m_tokVec[m_tokIdx].lineNum;
	while (curLineNum == m_tokVec[m_tokIdx].lineNum && m_tokVec[m_tokIdx].type != OPEN_BRACE)
		tokSet.push_back(m_tokVec[m_tokIdx++].str);
	if (info.createLimitExcept(tokSet))
		throw WsException(m_tokVec[m_tokIdx - 1].lineNum, "invalid limit except method");
	return (true);
}


