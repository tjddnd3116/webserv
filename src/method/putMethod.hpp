#ifndef putMethod_hpp
#define putMethod_hpp

#include "AMethod.hpp"

class putMethod : public AMethod
{
	private:
		std::string	m_bodyBuffer;
		int32_t		m_bodySize;
		std::string m_bodyType;

	public:
		putMethod(const std::string& readLine, const configInfo& conf);
		~putMethod();

		virtual void	loadRequest(const std::string& readLine);
		virtual const std::string&
						getBody(void) const;
		virtual bool	checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool	isMethodCreateFin(void) const;
		virtual void	logMethodInfo(std::fstream& logFile) const;

		void			loadBody(const std::string& readLine);
		void			getBodyType(void);

};
#endif //putMethod_hpp
