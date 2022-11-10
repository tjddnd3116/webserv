#ifndef postMethod_hpp
#define postMethod_hpp

#include "AMethod.hpp"

class cgi;

class postMethod : public AMethod
{
	private:
		std::string		m_bodyBuffer;
		std::string		m_bodyType;
		std::string		m_readBody;
		int32_t			m_bodySize;
		int32_t			m_readLineSize;
		cgi*			m_cgi;

		std::string		m_tempBuffer;
		int				m_testcnt;

	public:
		postMethod(const std::string& method, const configInfo& conf);
		~postMethod();

		virtual const std::string&		getReadBody(void) const;
		virtual void					loadRequest(const std::string& readLine);
		virtual const std::string&		getBody(void) const;
		virtual bool					checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool					isMethodCreateFin(void);
		virtual void					logMethodInfo(std::fstream& logFile) const;
		virtual void					uriParse(void);
		virtual void					doMethodWork(void);
		virtual void					filePathParse(std::string uri);

		void							loadBody(const std::string& readLine);
		void							getBodyType(void);
		bool							extractChuncked(std::string const& message);
};
#endif //postMethod_hpp
