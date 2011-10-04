#include "logger/logger.h"

#include <iostream>
#include <fstream>
#include <queue>

#define LOG_TO_FILE

namespace Typhon
{
#ifdef LOG_TO_FILE
	std::queue<std::string> logMessageQueue;
#endif

	void Log(const std::string &msg)
	{
#ifdef LOG_TO_FILE
		logMessageQueue.push(msg);
#else		
		std::cout << msg << std::endl;
#endif
	}

	void Logger::Flush()
	{
#ifdef LOG_TO_FILE
		std::ofstream logFile("log.txt");
		if(logFile.is_open())
		{
			while(!logMessageQueue.empty())
			{
				std::cout << logMessageQueue.front() << "\n";
				logFile << logMessageQueue.front() << std::endl;
				logMessageQueue.pop();
			}
			std::cout << "\nFinished writing log file.\n";
		}
		else
		{
			std::cout << "Unable to open log file and write contents.\n";
		}
#endif
	}
}
