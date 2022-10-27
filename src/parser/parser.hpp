#ifndef parser_hpp
#define parser_hpp

#include <vector>
#include <cstring>
#include <iostream>

#include "configInfo.hpp"
#include "fileReader.hpp"
#include "tokenizer.hpp"

class parser
{
	private:
		std::vector<std::string>	m_configBuffer;
		fileReader					m_fileReader;
		tokenizer					m_tokenizer;

		// hide copy
		parser(const parser& copy);
		parser& operator=(const parser& copy);

	public:
		// constructor & destructor
		parser(const char* path);
		~parser();

		// parsing
		void	makeToken(void);
		void	parse(std::vector<configInfo>& configInfo,
					  std::fstream& logFile);
		void	configParse(std::vector<configInfo>& configInfo,
							std::fstream& logFile);

};

#endif //parser_hpp
