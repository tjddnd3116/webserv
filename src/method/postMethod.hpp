#ifndef postMethod_hpp
#define postMethod_hpp

#include "AMethod.hpp"

class postMethod : public AMethod
{
	private:
		bool		m_isBody;
		std::string	m_bodyBuffer;

	public:
		postMethod(const std::string& method, const configInfo& conf);
		~postMethod();

		virtual void	loadRequest(const std::string& readLine);
		virtual const std::string&
						getBody(void) const;
		virtual bool	checkMethodLimit(const std::vector<std::string>& limitExcept) const;

		void			printBody(void) const;
		void			loadBody(const std::string& readLine);
};
#endif //postMethod_hpp
