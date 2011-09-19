#include "network/networklinux.h"

#ifndef WIN32

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <cstdio>

using namespace std;

namespace Typhon
{
	void NetworkLinux::DisplayError(const std::string &message)
	{
		// TODO hopefully perror is thread-safe...
		perror(message.c_str());
	}

	std::string NetworkLinux::GetIP()
	{
		ifaddrs *iflist, *iface;
		if(getifaddrs(&iflist) < 0)
		{
			DisplayError("getifaddrs");
			return "";
		}

		string ipaddr;
		bool done = false;
		for(iface = iflist; !done && iface; iface = iface->ifa_next)
		{
			int af = iface->ifa_addr->sa_family;
			switch(af)
			{
			case AF_INET:
				char addr[INET_ADDRSTRLEN];
				ipaddr += inet_ntop(af, &(reinterpret_cast<sockaddr_in*>(iface->ifa_addr)->sin_addr), addr, sizeof(addr));
				done = true;
				break;
			default:
				break;
			}
		}

		freeifaddrs(iflist);
		return ipaddr;
	}

	std::string NetworkLinux::GetNetMask()
	{
		string mask;

		return mask;
	}

	NetworkLinux::NetworkLinux(const int port) :
		NetworkRaw(port)
	{

	}

	NetworkLinux::~NetworkLinux()
	{

	}

	void NetworkLinux::BroadcastMessage(const std::string &msg, const char prefix)
	{
	}

	const Message NetworkLinux::ReceiveMessage()
	{

		Message m;
		m.prefix = 'N';
		return m;
	}

	bool NetworkLinux::StartUp()
	{
		return true;
	}
}

#endif