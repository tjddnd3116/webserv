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
		bool						m_isEof;
		size_t						m_line;

		int			checkBufPos(void);
		void		readToBuf(void);

		// hide copy
		fileReader(const fileReader& copy);
		fileReader& operator=(const fileReader& copy);

	public:
		// constructor & destructor
		fileReader();
		~fileReader();

		void							initFileReader(const char* path);
		t_token							readFile(void);
		bool							isEof(void) const;

		// getter
		const std::vector<std::string>& getAllBuffer(void);


};
#endif //fileReader_hpp
