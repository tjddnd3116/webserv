#ifndef postMethod_hpp
#define postMethod_hpp

#include "AMethod.hpp"

class postMethod : public AMethod
{
	private:
		std::string		m_readBody;
		int32_t			m_readLineSize;
		std::string		m_tempBuffer;

	public:
		postMethod(const std::string& method, const configInfo& conf);
		~postMethod();

		virtual std::string&			getReadBody(void);
		virtual void					loadRequest(const std::string& readLine);
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
