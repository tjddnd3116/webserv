#ifndef deleteMethod_hpp
#define deleteMethod_hpp

#include "AMethod.hpp"

class deleteMethod : public AMethod
{
	private:
		std::string		m_readBody;
	public:
		deleteMethod(const std::string& readLine, const configInfo& conf);
		~deleteMethod();

		virtual const std::string&		getReadBody(void) const;
		virtual void					loadRequest(const std::string &readLine);
		virtual bool					checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool					isMethodCreateFin(void) const;
		virtual void					logMethodInfo(std::fstream& logFile) const;
		virtual void					uriParse(void);
		virtual void					doMethodWork(void);

};
#endif //deleteMethod_hpp
