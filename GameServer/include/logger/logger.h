#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace Typhon
{
	class Logger
	{
		Logger();
		~Logger();

	public:

		static void Flush();
	};

	void Log(const std::string &msg);
}

#endif
