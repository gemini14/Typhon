#include "network/network.h"

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

namespace Typhon
{
	int Network::portNumber;
	const int recvBufferLength = 1024;

	sockaddr_in Network::machineAddr;
	sockaddr_in Network::broadcastAddr;


	Network::Network(const int port)
	{
		portNumber = port;
	}

	Network::~Network()
	{
	}

	const unsigned long Network::GetIP() const
	{
#ifdef WIN32
		return machineAddr.sin_addr.S_un.S_addr;
#else
		// TODO: Add linux equivalent
#endif
	}

	std::string Network::GetIPInStringForm() const
	{
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &machineAddr.sin_addr, str, INET_ADDRSTRLEN);
		return str;
	}
}