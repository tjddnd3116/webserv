#include "configParser.hpp"
#include "tokenizer.hpp"

configParser::configParser(const char* path)
{
	m_fileReader.initFileReader(path);
}

configParser::~configParser()
{}

configParser::configParser(const configParser& copy)
{
	*this = copy;
}

configParser&
configParser::operator=(const configParser& copy)
{
	m_fileReader = copy.m_fileReader;
	m_tokenizer = copy.m_tokenizer;
	m_allConfigBuffer = copy.m_allConfigBuffer;
	return (*this);
}

void
configParser::initParser(void)
{
	t_token		token;
	tokenizer	tokenizer;

	while (1)
	{
		if (m_fileReader.isEof())
			break;
		token = m_fileReader.readFile();
		m_tokenizer.pushBackToken(token);
	}
	m_allConfigBuffer = m_fileReader.getAllBuffer();
}

void configParser::parse(WsInitializer& init)
{
	try
	{
		m_tokenizer.parseToken(init);
	}
	catch (WsException& e)
	{
		e.printConfigErr(m_allConfigBuffer);
		throw WsException("invalid config file");
	}

}
