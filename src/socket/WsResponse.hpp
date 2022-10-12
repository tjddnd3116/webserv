#ifndef WsResponse_hpp
#define WsResponse_hpp

#include <string>
#include <fstream>
#include <ctime>

#include "../method/WsIMethod.hpp"
#include "../parser/WsConfigInfo.hpp"

class WsResponse
{
	private:
		std::fstream			m_file;
		std::string				m_responseBuf;
		const WsIMethod*		m_method;
		WsConfigInfo			m_conf;
		int						m_statusCode;

		void				makeStatusLine(void);
		void				makeResponseHeader(void);
		void				makeEntityHeader(void);
		void				makeGeneralHeader(void);
		const std::string	getDate(void);

	public:
		WsResponse(const WsConfigInfo& conf);
		~WsResponse();
		WsResponse(const WsResponse& copy);
		WsResponse& operator=(const WsResponse& copy);

		void	makeBody(void);
		void	makeResponse(const WsIMethod* method);
		void	makeDate(void);
		size_t	getBufSize(void) const;

		const std::string& operator()(void);
};
#endif //WsResponse_hpp
