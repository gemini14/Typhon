#ifndef LOGGER_H
#define LOGGER_H

#include <queue>
#include <string>

namespace Typhon
{
	class Logger
	{
		static bool flushExit;
	
		Logger();
		~Logger();

	public:

		static void FlushAndExit();
		static void Run();
	};

	void Log(const std::string &msg);
}

#endif