#ifndef configParser_hpp
#define configParser_hpp

#include <vector>
#include <cstring>
#include <iostream>

#include "WsConfigInfo.hpp"
#include "fileReader.hpp"
#include "tokenizer.hpp"

class configParser
{
	private:
		fileReader m_fileReader;
		tokenizer m_tokenizer;
		std::vector<std::string> m_allConfigBuffer;

	public:
		configParser(const char* path);
		~configParser();
		configParser(const configParser& copy);
		configParser& operator=(const configParser& copy);

		void	initParser(void);
		void	parse(WsInitializer& init);
};

#endif //configParser_hpp
