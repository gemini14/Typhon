#ifndef STATEEXCEPTION_H
#define STATEEXCEPTION_H

#include <exception>

namespace Typhon
{
	class StateException : public std::exception
	{
	private:
		std::string msg;

	public:
		StateException(const char *msg)
			: msg(msg)
		{
		}

		virtual ~StateException() throw()
		{
		}

		virtual const char* what() const throw ()
		{
			return msg.c_str();
		}
	};
}

#endif