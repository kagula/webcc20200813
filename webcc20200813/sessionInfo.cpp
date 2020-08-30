#include "sessionInfo.h"

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

namespace kagula
{
	boost::mutex m_mutexNewSessionID;
	std::string NewSessionID()
	{
		boost::lock_guard<boost::mutex> lock(m_mutexNewSessionID);
		static unsigned sessionIndex = 0;
		return boost::lexical_cast<std::string>(++sessionIndex);
	}
}