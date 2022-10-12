#ifndef WsInitializer_hpp
#define WsInitializer_hpp

#include <string>
#include <vector>

#include "./parser/WsConfigInfo.hpp"
#include "./parser/configParser.hpp"

#define BUFFER_SIZE 1024

class WsInitializer
{
	private:
		// member variable
		std::vector<WsConfigInfo>	m_conf;

	public:
		// Orthodox Canonical Form
		WsInitializer();
		~WsInitializer();
		WsInitializer(const WsInitializer& copy);
		WsInitializer	operator=(const WsInitializer& copy);


		void						parseConfig(const char* path);
		void						pushBack(WsConfigInfo &info);
		std::vector<WsConfigInfo>	getConfigInfo(void);
};
#endif //WsInitializer_hpp
