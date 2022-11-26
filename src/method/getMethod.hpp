#ifndef getMethod_hpp
#define getMethod_hpp

#include "AMethod.hpp"

class getMethod : public AMethod
{
	private:
		std::string		m_readBody;
		bool			m_isAutoIdx;

	public:
		getMethod(const std::string& readLine, const configInfo& conf);
		~getMethod();

		virtual std::string&			getReadBody(void);
		virtual void					loadRequest(const std::string &readLine);
		virtual bool					checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool					isMethodCreateFin(void);
		virtual void					logMethodInfo(std::fstream& logFile) const;
		virtual void					uriParse(void);
		virtual void					doMethodWork(void);
		virtual void					filePathParse(std::string uri);

};
#endif //getMethod_hpp
