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
}