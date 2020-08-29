#ifndef _SESSION_SUPPORT_H_
#define _SESSION_SUPPORT_H_

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "sessionInfo.h"

/*
功能：提供Cookie支持，通过Cookie中存放Session ID识别每个不同的用户，维护每个session对应的具体用户状态。
依赖：Boost::ASIO库 示例代码 http::server4中的代码段.
*/
namespace kagula
{
	class SessionSupport
	{
	public:
		~SessionSupport();

		static SessionSupport& Instance()
		{
			static SessionSupport ss;
			return ss;
		}

		//响应客户端请求的时候调用，返回session id的具体值，
		//如果客户端没有在Cookie中上传session id就分配一个新的sessionid.
		//后面的代码可以通过这个session id维护当前session的用户信息。
		std::string parseReq(const std::string& cookieValue);

		//供处理用户请求的时候调用。
		boost::shared_ptr<SessionInfo> getSessionInfo(const std::string& sessionID);


		//返回信息给客户端之前调用，用来告诉浏览器本服务端的Cookie值设置。
		std::string getSetCookieValue(const std::string& sessionID);

		void removeSession(const std::string& sessionID);
	private:
		//key是sessionID
		std::map<std::string, boost::shared_ptr<SessionInfo>> m_si;

		// Other non - static member functions
	private:
		SessionSupport();                                    // Private constructor
		SessionSupport(const SessionSupport&);               // Prevent copy-construction
		SessionSupport& operator=(const SessionSupport&);    // Prevent assignment

		void checkTimeout();

		boost::shared_mutex m_mutexRW;
		boost::thread m_tCheck;
	};
}
#endif