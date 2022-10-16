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
		fileReader					m_fileReader;
		tokenizer					m_tokenizer;
		std::vector<std::string>	m_configBuffer;
		std::vector<configInfo>		m_config;

		// can't copy
		// just create to use default constructor
		parser(const parser& copy);
		parser& operator=(const parser& copy);

	public:
		parser(const char* path);
		~parser();

		void							makeToken(void);
		void							parse(void);
		void							configParse(void);
		const std::vector<configInfo>&	getConfigInfo(void);
};

#endif //parser_hpp
