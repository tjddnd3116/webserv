#ifndef tokenizer_hpp
#define tokenizer_hpp

#include <string>
#include <sys/resource.h>
#include <vector>
#include <iostream>
#include <exception>

#include "WsConfigInfo.hpp"
#include "../WsException.hpp"

class WsInitializer;

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
		void	 	serverParse(WsConfigInfo &wsConfigInfo);
		void		serverContextParse(WsConfigInfo &info);
		void		locationContextParse(WsConfigInfo &info);
		void		locationParse(WsConfigInfo &info);
		bool		verifyInfo(WsConfigInfo& info);
		bool		isSafeIdx(void);
		bool		isComment(const t_token& token) const;
		bool		isOpenBrace(void);
		bool		isCloseBrace(void);
		bool		isOptionContext(void);
		bool		isSemicolon(size_t optionLineNum);
		bool		isLocationPath(WsConfigInfo& info);

	public:
		tokenizer();
		~tokenizer();
		tokenizer(const tokenizer& copy);
		tokenizer& operator=(const tokenizer& copy);

		void		pushBackToken(t_token& token);
		void	 	parseToken(WsInitializer &initializer);

};

#endif //tokenizer_hpp
