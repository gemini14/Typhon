#ifndef NETWORKFACTORY_H
#define NETWORKFACTORY_H

#include "network.h"

namespace Typhon
{
	enum NETWORK_HANDLING_TYPE
	{
		RAW,
		ENET
	};

	Network* GetNetwork(NETWORK_HANDLING_TYPE type, const int port);
}

#endif