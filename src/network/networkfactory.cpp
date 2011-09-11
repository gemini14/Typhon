#include "network/networkfactory.h"

#ifdef WIN32
#include "network/networkwin.h"
#else
#include "network/networklinux.h"
#endif

#include "network/networkenet.h"

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
		else
		{
			if(IP)
			{
				net = new NetworkENet(port, IP);
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
