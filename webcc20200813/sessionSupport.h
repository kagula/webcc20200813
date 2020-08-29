#ifndef _SESSION_SUPPORT_H_
#define _SESSION_SUPPORT_H_

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "sessionInfo.h"

/*
���ܣ��ṩCookie֧�֣�ͨ��Cookie�д��Session IDʶ��ÿ����ͬ���û���ά��ÿ��session��Ӧ�ľ����û�״̬��
������Boost::ASIO�� ʾ������ http::server4�еĴ����.
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

		//��Ӧ�ͻ��������ʱ����ã�����session id�ľ���ֵ��
		//����ͻ���û����Cookie���ϴ�session id�ͷ���һ���µ�sessionid.
		//����Ĵ������ͨ�����session idά����ǰsession���û���Ϣ��
		std::string parseReq(const std::string& cookieValue);

		//�������û������ʱ����á�
		boost::shared_ptr<SessionInfo> getSessionInfo(const std::string& sessionID);


		//������Ϣ���ͻ���֮ǰ���ã��������������������˵�Cookieֵ���á�
		std::string getSetCookieValue(const std::string& sessionID);

		void removeSession(const std::string& sessionID);
	private:
		//key��sessionID
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