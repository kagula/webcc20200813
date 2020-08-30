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

	//�����ֻ��Ҫ�ڿͻ���Cookie�д��session id��ֵ�����session id��Ӧ�ľ�������
	//�ɷ����ά����
	const char session_key[] = "CSessionID";

	std::string NewSessionID();

	struct SessionInfo
	{
		SessionInfo(std::string sessionID) :count(0), sessionid(sessionID) {}

		//����û����ơ��û���ɫ���͵��û���Ϣ��
		//��������Ĵ����֪����ǰsession���û���Ϣ��
		std::map<std::string, std::string> session;

		//������һ��XX.html����GET Method��ʽ����������������
		//Ӧ����ÿ��ִ�о���������ǰ���á�
		std::map<std::string, std::map<std::string, std::string>> sessionPage;

		//
		const std::string sessionid;
		unsigned count;//ÿ��+1���������3600s�e��, ��ǰSession�������
	};
}

#endif
