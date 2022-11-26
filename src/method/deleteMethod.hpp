#ifndef deleteMethod_hpp
#define deleteMethod_hpp

#include "AMethod.hpp"
#include <unistd.h>

class deleteMethod : public AMethod
{
	private:
		std::string		m_readBody;
	public:
		deleteMethod(const std::string& readLine, const configInfo& conf);
		~deleteMethod();

		virtual std::string&			getReadBody(void);
		virtual void					loadRequest(const std::string &readLine);
		virtual bool					checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool					isMethodCreateFin(void);
		virtual void					logMethodInfo(std::fstream& logFile) const;
		virtual void					uriParse(void);
		virtual void					doMethodWork(void);
		virtual void					filePathParse(std::string uri);
};
#endif //deleteMethod_hpp
