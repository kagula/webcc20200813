#ifndef _SESSIONINFO_HPP_
#define _SESSIONINFO_HPP_

#ifdef _WIN32
#pragma once
#endif

#include <string>
#include <map>

namespace kagula
{
	const char req_cookie[] = "Cookie";
	const char reply_set_cookie[] = "Set-Cookie";

	//服务端只需要在客户端Cookie中存放session id的值，这个session id对应的具体数据
	//由服务端维护。
	const char session_key[] = "CSessionID";

	std::string NewSessionID();

	struct SessionInfo
	{
		SessionInfo(std::string sessionID) :count(0), sessionid(sessionID) {}

		//存放用户名称、用户角色类型等用户信息。
		//这样后面的代码就知道当前session的用户信息。
		std::map<std::string, std::string> session;

		//存放最后一次XX.html请求，GET Method方式后面跟的请求参数。
		//应该在每次执行具体请求处理前调用。
		std::map<std::string, std::map<std::string, std::string>> sessionPage;

		//
		const std::string sessionid;
		unsigned count;//每秒+1，如果超过3600s閑置, 当前Session被清除。
	};
}

#endif
