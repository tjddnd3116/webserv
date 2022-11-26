#ifndef headMethod_hpp
#define headMethod_hpp

#include "AMethod.hpp"

class headMethod : public AMethod
{
	private:
		std::string		m_readBody;

	public:
		headMethod(const std::string& readLine, const configInfo& conf);
		~headMethod();

		virtual std::string&			getReadBody(void);
		virtual void					loadRequest(const std::string &readLine);
		virtual bool					checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool					isMethodCreateFin(void);
		virtual void					logMethodInfo(std::fstream& logFile) const;
		virtual void					uriParse(void);
		virtual void					doMethodWork(void);
		virtual void					filePathParse(std::string uri);
};
#endif //headMethod_hpp
