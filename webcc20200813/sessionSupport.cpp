#include "SessionSupport.h"
//#include "request.hpp"

#include <boost/smart_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

namespace kagula
{
	SessionSupport::SessionSupport()
	{
		boost::thread t(boost::bind(&SessionSupport::checkTimeout, this));
		m_tCheck = boost::move(t);
	}


	SessionSupport::~SessionSupport()
	{
		m_tCheck.interrupt();
		m_tCheck.join();
	}

	void SessionSupport::checkTimeout()
	{
		while (true)
		{
			{
				boost::unique_lock<boost::shared_mutex> lock(m_mutexRW, boost::defer_lock);
				if (lock.try_lock())
				{
					std::map<std::string, boost::shared_ptr<SessionInfo>>::iterator iter = m_si.begin();
					while (iter != m_si.end())
					{
						iter->second->count++;
						//if a session have not accessed by a client exceed an hour, erase it.
						if (iter->second->count > 3600)
						{
							m_si.erase(iter++);
							continue;
						}//if
						iter++;
					}//while
					lock.unlock();
				}//if
			}//check session

			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}//while
	}//function

	std::string SessionSupport::parseReq(const std::string& cookieValue)
	{
		std::string sessionID;
		//extract session id from cookie value.
		if (cookieValue.empty() == false)
		{
			std::vector<std::string> vecRec;
			boost::split(vecRec, cookieValue, boost::is_any_of(";"));
			for (unsigned i = 0; i < vecRec.size(); i++)
			{
				std::vector<std::string> vecSub;
				boost::split(vecSub, vecRec[i], boost::is_any_of("="));
				if (vecSub.size() == 2)
				{
					boost::trim_if(vecSub[0], boost::is_any_of("\" \n\r\t'"));
					boost::trim_if(vecSub[1], boost::is_any_of("\" \n\r\t'"));

					if (vecSub[0] == session_key)
					{
						sessionID = vecSub[1];
						break;
					}
				}//end if
			}//end for
		}//end if

		//
		if (cookieValue.empty() || sessionID.empty() ||
			m_si.find(sessionID) == m_si.end())//����������������رգ�û�м�ʱ���sessionID�����ϴΡ��Ѿ���Ч�ġ�session idҲ�������ˡ�
		{
			boost::lock_guard<boost::shared_mutex> lock(m_mutexRW);
			do
			{
				sessionID = NewSessionID();
			} while (m_si.find(sessionID) != m_si.end());

			boost::shared_ptr<SessionInfo> pSI(new SessionInfo(sessionID));
			m_si[sessionID] = pSI;
		}
		return sessionID;
	}

	//�������û������ʱ����á�
	boost::shared_ptr<SessionInfo> SessionSupport::getSessionInfo(const std::string& sessionID)
	{
		boost::lock_guard<boost::shared_mutex> lock(m_mutexRW);
		m_si[sessionID]->count = 0;
		return m_si[sessionID];
	}


	void SessionSupport::removeSession(const std::string& sessionID)
	{
		boost::lock_guard<boost::shared_mutex> lock(m_mutexRW);
		m_si.erase(sessionID);
	}


	//������Ϣ���ͻ���֮ǰ���ã��������������������˵�Cookieֵ���á�
	std::string SessionSupport::getSetCookieValue(const std::string& sessionID)
	{
		std::string strCookieValue;
		strCookieValue = session_key;
		strCookieValue.append("=");
		strCookieValue.append(sessionID);
		return strCookieValue;
	}

}