#ifndef NETWORKFACTORY_H
#define NETWORKFACTORY_H

#include "network.h"

namespace Typhon
{
	const int PORT_NUMBER = 1550;

	enum NETWORK_HANDLING_TYPE
	{
		ENETSERVER
	};

	Network* GetNetwork(NETWORK_HANDLING_TYPE type, const int port, const sockaddr_in *IP = nullptr);
}

#endif
