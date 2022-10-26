#ifndef WsResponse_hpp
#define WsResponse_hpp

#include <string>
#include <fstream>
#include <ctime>
#include <map>

#include "../method/AMethod.hpp"
#include "../parser/configInfo.hpp"
#include "../cgi/cgi.hpp"

class response
{
	private:
		std::fstream		m_file;
		std::string			m_responseBuf;
		std::string			m_newBody;
		std::string			m_type;
		const AMethod*		m_method;
		configInfo			m_conf;

		// 필요없음
		int					m_statusCode;
		std::string 		m_fileExt;
		int					m_isCgi;
		std::string			m_filePath;

		std::string			m_rootPath;


		int 				check_isCgi();
		void				makeStatusLine(void);
		void				makeResponseHeader(void);
		void				makeEntityHeader(void);
		void				makeGeneralHeader(void);
		void				extractExt();
		void				parseBody();
		const std::string	getDate(void);
		const std::string	getStatusCodeStr(void);

		response(const response& copy);
		response& operator=(const response& copy);
	public:
		response(const configInfo& conf);
		~response();

		static std::map<int, std::string>	s_statusCode;

		static void	setStatusCode(void);
		void		makeBody(void);
		void		makeResponse(const AMethod* method);
		void		makeDate(void);
		size_t		getBufSize(void) const;
		const std::string& operator()(void);

};
#endif //response_hpp
