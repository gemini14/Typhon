#include "network/networkfactory.h"

#ifdef WIN32
#include "network/networkwin.h"
#else
#include "network/networklinux.h"
#endif

#include "network/networkenetclient.h"
#include "network/networkenetserver.h"

namespace Typhon
{
	Network* GetNetwork(NETWORK_HANDLING_TYPE type, const int port, const sockaddr_in *IP)
	{
		Network *net = nullptr;

		switch(type)
		{
		case RAW:
#ifdef WIN32
			net = new NetworkWin(port);
#else
			net = new NetworkLinux(port);
#endif
			break;

		case ENETCLIENT:
			if(IP)
			{
				net = new NetworkENetClient(port, IP);
			}
			break;

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
