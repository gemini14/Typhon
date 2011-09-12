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
		if (type == RAW)
		{
#ifdef WIN32
			net = new NetworkWin(port);
#else // Linux
			net = new NetworkLinux(port);
#endif
		}
		else if (IP)
		{
			if(type == ENETCLIENT)
			{
				net = new NetworkENetClient(port, IP);
			}
			else
			{
				net = new NetworkENetServer(port);
			}
		}

		if (net && !net->StartUp())
		{
			if(net)
			{
				delete net;
			}
			return nullptr;
		}

		return net;
	}
}
