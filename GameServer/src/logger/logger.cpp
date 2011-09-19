#include "logger/logger.h"

#include <iostream>
#include <queue>

namespace Typhon
{
	// std::queue<std::string> logMessageQueue;

	void Log(const std::string &msg)
	{
		// logMessageQueue.push(msg);
		std::cout << msg << std::endl;
	}

	void Logger::Flush()
	{
		// left in in case dumping to file through iostreams, Lua, or Python
		// is desired in future

		/*
		while(true)
		{
			if(!logMessageQueue.empty())
			{
				std::cout << logMessageQueue.front() << std::endl;
				logMessageQueue.pop();
			}
		}
		*/
	}
}
