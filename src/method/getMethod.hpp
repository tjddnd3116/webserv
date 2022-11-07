#ifndef getMethod_hpp
#define getMethod_hpp

#include "AMethod.hpp"

class getMethod : public AMethod
{
	private:
		std::string		m_readBody;

	public:
		getMethod(const std::string& readLine, const configInfo& conf);
		~getMethod();

		virtual const std::string&		getReadBody(void) const;
		virtual void					loadRequest(const std::string &readLine);
		virtual bool					checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool					isMethodCreateFin(void);
		virtual void					logMethodInfo(std::fstream& logFile) const;
		virtual void					uriParse(void);
		virtual void					doMethodWork(void);

};
#endif //getMethod_hpp
