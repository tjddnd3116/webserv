#ifndef putMethod_hpp
#define putMethod_hpp

#include "AMethod.hpp"

class putMethod : public AMethod
{
	private:
		std::string	m_bodyBuffer;
		std::string	m_readBody;
		int32_t		m_readLineSize;

	public:
		putMethod(const std::string& readLine, const configInfo& conf);
		~putMethod();

		virtual std::string&			getReadBody(void) ;
		virtual void					loadRequest(const std::string& readLine);
		virtual bool					checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool					isMethodCreateFin(void);
		virtual void					logMethodInfo(std::fstream& logFile) const;
		virtual void					uriParse(void);
		virtual void					doMethodWork(void);
		virtual void					filePathParse(std::string uri);

		void							loadBody(const std::string& readLine);
		void							getBodyType(void);

};
#endif //putMethod_hpp
