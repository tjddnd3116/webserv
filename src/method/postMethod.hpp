#ifndef postMethod_hpp
#define postMethod_hpp

#include "AMethod.hpp"

class postMethod : public AMethod
{
	private:
		bool		m_isBody;
		std::string	m_bodyBuffer;

		void	loadBody(const std::string& readLine);

	public:
		postMethod(const std::string& method, const configInfo& conf);
		~postMethod();

		virtual void	loadRequest(const std::string& readLine);
		void			printBody(void) const;
};
#endif //postMethod_hpp
