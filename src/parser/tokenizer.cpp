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
tokenizer::parseToken(std::vector<configInfo>& config,
					  std::fstream& logFile)
{
	while (m_tokIdx != m_tokVec.size() && m_tokVec[m_tokIdx].type == SERVER)
	{
		configInfo info;

		blockParse(info);
		info.createDefaultLocation();
		info.checkConfig();
		logFile << info;
		info.locationVecToMap();
		config.push_back(info);
	}
	if (m_tokIdx != m_tokVec.size())
		throw WsException(m_tokVec[m_tokIdx].lineNum, "invaild server block");
}

void
tokenizer::contextParse(configInfo &info,
						const std::string& blockType)
{
	std::vector<std::string>	tokenSet;
	size_t						optionLineNum;

	m_tokIdx++;
	while (isOptionContext())
	{
		if (isLocationBlock(blockType))
		{
			m_tokIdx++;
			if (isLocationPath(info))
				blockParse(info, "loc_");
			continue ;
		}
		optionLineNum = m_tokVec[m_tokIdx].lineNum;
		std::string &tokenStr = m_tokVec[m_tokIdx++].str;
		if (configInfo::s_table[blockType + tokenStr] == 0)
			throw WsException(m_tokVec[m_tokIdx - 1].lineNum, "invaild block option");
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
		try {
			(info.*(configInfo::s_table[blockType + tokenStr]))(tokenSet);
		} catch (WsException& e) {
			std::string errMsg = e.getErrorMsg();
			throw WsException(m_tokVec[m_tokIdx - 1].lineNum, errMsg);
		}
	}
}

void
tokenizer::blockParse(configInfo &info,
					  const std::string& blockType)
{
	m_tokIdx++;
	if (isOpenBrace())
		contextParse(info, blockType);
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
tokenizer::isLocationBlock(const std::string& blockType)
{
	if (m_tokVec[m_tokIdx].str == "location")
	{
		if (blockType == "loc_")
				throw WsException(m_tokVec[m_tokIdx].lineNum - 1, "miss '}'");
		return (true);
	}
	return (false);
}
