#include "logger/logger.h"

#include <iostream>
#include <queue>

#include <boost/thread.hpp>

namespace Typhon
{
	boost::mutex logQueueGuard;
	boost::mutex flushExitGuard;

	bool Logger::flushExit;

	std::queue<std::string> logMessageQueue;

	void Log(const std::string &msg)
	{
#if defined(DEBUG)|defined(_DEBUG)
		boost::lock_guard<boost::mutex> lock(logQueueGuard);
		logMessageQueue.push(msg);
#endif
	}

	void Logger::FlushAndExit()
	{
		boost::lock_guard<boost::mutex> lock(flushExitGuard);
		flushExit = true;
	}

	void Logger::Run()
	{
		{
			boost::lock_guard<boost::mutex> lock(flushExitGuard);
			flushExit = false;
		}

		while(true)
		{
			{
				boost::lock_guard<boost::mutex> lock(logQueueGuard);
				if(!logMessageQueue.empty())
				{
					std::cout << logMessageQueue.front() << std::endl;
					logMessageQueue.pop();
				}
			}

			boost::lock_guard<boost::mutex> lock(flushExitGuard);
			if(flushExit)
			{
				boost::lock_guard<boost::mutex> lock(logQueueGuard);
				while(!logMessageQueue.empty())
				{
					std::cout << logMessageQueue.front() << std::endl;
					logMessageQueue.pop();
				}
				break;
			}
		}
	}
}