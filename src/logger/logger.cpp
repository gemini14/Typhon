#include "logger/logger.h"

#include <iostream>

#include <boost/thread.hpp>

namespace Typhon
{
	boost::mutex logQueueGuard;
	boost::mutex flushExitGuard;

	std::queue<std::string> logMessageQueue;

	void Log(const std::string &msg)
	{
		boost::lock_guard<boost::mutex> lock(logQueueGuard);
		logMessageQueue.push(msg);
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
					auto msg = logMessageQueue.front();
					std::cout << msg << std::endl;
				}
			}

			{
				boost::lock_guard<boost::mutex> lock(flushExitGuard);
				if(flushExit)
				{
					break;
				}
			}
		}
	}
}