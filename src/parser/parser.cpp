#include "parser.hpp"

parser::parser(const char* path)
{
	m_fileReader.initFileReader(path);
	configInfo::setTable();
}

parser::~parser()
{}

void
parser::makeToken(void)
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
	m_configBuffer = m_fileReader.getAllBuffer();
}

void
parser::parse(std::vector<configInfo>& configInfo,
			  std::fstream& logFile)
{
	try {
		m_tokenizer.parseToken(configInfo, logFile);
	} catch (WsException& e) {
		e.printConfigErr(m_configBuffer, logFile);
		throw WsException("invalid config file");
	}
}

void
parser::configParse(std::vector<configInfo>& configInfo,
					std::fstream& logFile)
{
	makeToken();
	parse(configInfo, logFile);
}
