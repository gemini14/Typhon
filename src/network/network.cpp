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

	std::string Network::GetIPInStringForm()
	{
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &machineAddr.sin_addr, str, INET_ADDRSTRLEN);
		return str;
	}
}