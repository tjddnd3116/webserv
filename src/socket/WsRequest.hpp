#ifndef WsRequest_hpp
#define WsRequest_hpp

#include <iostream>
#include <string>
#include <vector>

#include "../method/WsGetMethod.hpp"
#include "../method/WsPostMethod.hpp"
#include "../method/WsPutMethod.hpp"

class WsRequest
{
		private:
			WsIMethod* m_method;

		public:
			WsRequest();
			~WsRequest();

			void				readRequest(const std::string& request);
			WsIMethod*			methodGenerator(const std::string& readLine);
			const WsIMethod*	getMethod(void) const;

			void				printRequest(void) const;
};
#endif //WsRequest_hpp
