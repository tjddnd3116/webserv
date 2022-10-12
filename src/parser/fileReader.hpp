#ifndef fileReader_hpp
#define fileReader_hpp

#include <string>
#include <fstream>
#include <iostream>
#include <cctype>

#include "../WsException.hpp"
#include "tokenizer.hpp"

class fileReader
{
	private:
		std::vector<std::string>	m_allBuffer;
		std::ifstream				m_configFile;
		std::string					m_buffer;
		size_t						m_pos;
		bool						m_eof;
		size_t						m_line;

		int			checkBufPos(void);
		void		readToBuf(void);
	public:
		fileReader();
		~fileReader();
		fileReader(const fileReader& copy);
		fileReader& operator=(const fileReader& copy);

		void		initFileReader(const char* path);
		t_token		readFile(void);
		bool		isEof(void) const;
		const std::vector<std::string>&
					getAllBuffer(void);


};
#endif //fileReader_hpp
