#include "fileReader.hpp"

fileReader::fileReader()
{
	m_buffer.clear();
	m_pos = 0;
	m_eof = false;
	m_line = -1;
}

fileReader::~fileReader()
{}

fileReader::fileReader(const fileReader& copy)
{
	*this = copy;
}

fileReader&
fileReader::operator=(const fileReader& copy)
{
	m_allBuffer = copy.m_allBuffer;
	m_configFile.clear();
	m_buffer = copy.m_buffer;
	m_pos = copy.m_pos;
	m_eof = copy.m_eof;
	m_line = copy.m_line;
	return (*this);
}

void
fileReader::initFileReader(const char *path)
{
	m_configFile.open(path);
	if (m_configFile.fail())
		throw WsException("config file open failed");
}

t_token
fileReader::readFile(void)
{
	t_token		token;
	size_t		prePos;

	token.str = "";
	if (checkBufPos())
		return (token);
	prePos = m_pos;
	m_pos = m_buffer.find(" ", prePos);
	if (m_pos == std::string::npos)
		token.str.assign(m_buffer, prePos, m_buffer.size() - prePos);
	else
		token.str.assign(m_buffer, prePos, m_pos - prePos);
	token.str.erase(std::remove_if(token.str.begin(), token.str.end(), isspace), token.str.end());
	token.line = m_buffer;
	token.lineNum = m_line;
	return (token);
}

int		 fileReader::checkBufPos(void)
{
	if (m_configFile.eof() && m_pos == std::string::npos)
	{
		m_eof = true;
		m_configFile.close();
		return (1);
	}
	if (m_pos == std::string::npos || m_pos == 0)
		readToBuf();
	else
		m_pos++;
	return (0);
}

void	 fileReader::readToBuf(void)
{
	m_pos = 0;
	std::getline(m_configFile, m_buffer);
	m_allBuffer.push_back(m_buffer);
	m_line++;
}

bool	 fileReader::isEof(void) const
{
	return (m_eof);
}

const std::vector<std::string>& fileReader::getAllBuffer(void)
{
	return (m_allBuffer);
}
