#ifndef tokenizer_hpp
#define tokenizer_hpp

#include <sys/resource.h>

#include <string>
#include <vector>
#include <iostream>
#include <exception>

#include "configInfo.hpp"
#include "../WsException.hpp"

enum e_tokenType
{
	SERVER,
	OPEN_BRACE,
	CLOSE_BRACE,
	OPTION_CONTEXT,
	SEMICOLON
};

typedef struct	s_token
{
	std::string line;
	std::string	str;
	e_tokenType	type;
	size_t		lineNum;
}				t_token;

class tokenizer
{
	private:
		std::vector<t_token>	m_tokVec;
		size_t					m_tokIdx;

		e_tokenType	selectTokenType(const std::string& str) const;

		void	 	blockParse(configInfo &info, const std::string& blockType = "");
		void		contextParse(configInfo &info, const std::string& blockType);

		bool		isLocationBlock(const std::string& blockType);
		bool		isSafeIdx(void);
		bool		isComment(const t_token& token) const;
		bool		isOpenBrace(void);
		bool		isCloseBrace(void);
		bool		isOptionContext(void);
		bool		isSemicolon(size_t optionLineNum);
		bool		isLocationPath(configInfo& info);
		bool		isLimitExcept(configInfo& info);

		// hide copy
		tokenizer(const tokenizer& copy);
		tokenizer& operator=(const tokenizer& copy);

	public:
		// constructor & destructor
		tokenizer();
		~tokenizer();

		// make token & parse token
		void		pushBackToken(t_token& token);
		void	 	parseToken(std::vector<configInfo>& config, std::fstream& logFile);

};

#endif //tokenizer_hpp
