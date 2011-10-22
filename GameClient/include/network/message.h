#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#ifdef WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#else
#include <netinet/in.h>
#endif

namespace Typhon
{

	struct Message
	{
		char prefix;
		std::string msg;
		in_addr address;
	};

}

#endif