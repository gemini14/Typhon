#include "network/networkfactory.h"

#include "network/networkenetserver.h"

namespace Typhon
{
	Network* GetNetwork(NETWORK_HANDLING_TYPE type, const int port, const unsigned long IP)
	{
		Network *net = nullptr;

		switch(type)
		{
		case ENETSERVER:
			if(IP)
			{
				net = new NetworkENetServer(port, IP);
			}
			break;

		default:
			return nullptr;
		}

		if (net && !net->StartUp())
		{
			delete net;
			return nullptr;
		}

		return net;
	}
}
