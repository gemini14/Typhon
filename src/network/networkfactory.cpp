#include "network/networkfactory.h"

#ifdef WIN32
#include "network/networkwin.h"
#else
#include "network/networklinux.h"
#endif

#include "network/networkenet.h"

namespace Typhon
{
Network* GetNetwork(NETWORK_HANDLING_TYPE type, const int port)
{
	Network *net;
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
		net = new NetworkENet(port);
	}

	if (net && !net->StartUp())
	{
		return nullptr;
	}

	return net;
}
}
