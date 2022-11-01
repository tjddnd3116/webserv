#ifndef postMethod_hpp
#define postMethod_hpp

#include "AMethod.hpp"

class postMethod : public AMethod
{
	private:
		std::string	m_bodyBuffer;
		int32_t		m_bodySize;
		std::string m_bodyType;

	public:
		postMethod(const std::string& method, const configInfo& conf);
		~postMethod();

		virtual void	loadRequest(const std::string& readLine);
		virtual const std::string&
						getBody(void) const;
		virtual bool	checkMethodLimit(const std::vector<std::string>& limitExcept) const;
		virtual bool	isMethodCreateFin(void) const;
		virtual void	logMethodInfo(std::fstream& logFile) const;

		void			loadBody(const std::string& readLine);
		void			getBodyType(void);
};
#endif //postMethod_hpp
