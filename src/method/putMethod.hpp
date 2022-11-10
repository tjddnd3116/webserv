#ifndef putMethod_hpp
#define putMethod_hpp

#include "AMethod.hpp"

class putMethod : public AMethod
{
	private:
		std::string	m_bodyBuffer;
		std::string m_bodyType;
		std::string	m_readBody;
		int32_t		m_readLineSize;
		int32_t		m_bodySize;

	public:
		putMethod(const std::string& readLine, const configInfo& conf);
		~putMethod();

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

};
#endif //putMethod_hpp
