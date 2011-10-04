#include "network/networklinux.h"

#ifndef WIN32

#include <cstdio>
#include <cstring>

#include <arpa/inet.h>
#include <linux/if.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

using namespace std;

namespace Typhon
{
	void NetworkLinux::DisplayError(const std::string &message)
	{
		Log(message);
	}

	void NetworkLinux::Display_PError(const std::string &message)
	{
		perror(message.c_str());
	}

	std::string NetworkLinux::GetIP()
	{
		ifaddrs *iflist, *iface;
		if (getifaddrs(&iflist) == -1)
		{
			DisplayError("getifaddrs");
			return "";
		}

		string ipaddr;
		bool done = false;
		for (iface = iflist; !done && iface; iface = iface->ifa_next)
		{
			int af = iface->ifa_addr->sa_family;
			switch (af)
			{
				case AF_INET:
					if (!(iface->ifa_flags & IFF_LOOPBACK))
					{
						char addr[INET_ADDRSTRLEN];
						ipaddr.assign(
								inet_ntop(
										af,
										&(reinterpret_cast<sockaddr_in*>(iface->ifa_addr)->sin_addr),
										addr, sizeof(addr)));
						done = true;
					}
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
		ifaddrs *iflist, *iface;
		if (getifaddrs(&iflist) == -1)
		{
			Display_PError("getifaddrs");
			return "";
		}

		bool done = false;
		for (iface = iflist; !done && iface; iface = iface->ifa_next)
		{
			int af = iface->ifa_addr->sa_family;
			switch (af)
			{
				case AF_INET:
				{
					unsigned int ipaddr =
							reinterpret_cast<sockaddr_in*>(iface->ifa_addr)->sin_addr.s_addr;
					if (machineAddr.sin_addr.s_addr == ipaddr)
					{
						netmask = ((sockaddr_in*)iface->ifa_netmask)->sin_addr.s_addr;
						done = true;
					}
				}
					break;

				default:
					break;
			}
		}

		freeifaddrs(iflist);
		return "";
	}

	NetworkLinux::NetworkLinux(const int port) :
			NetworkRaw(port), linuxSocket(-1)
	{
	}

	NetworkLinux::~NetworkLinux()
	{
		close(linuxSocket);
	}

	void NetworkLinux::BroadcastMessage(const std::string &msg,
			const char prefix)
	{
		string data(prefix + msg);

		if (sendto(linuxSocket, data.c_str(), static_cast<int>(data.length()), 0,
		reinterpret_cast<sockaddr*>(&broadcastAddr), sizeof(broadcastAddr)) == -1)
		{
			Display_PError("sendto");
		}
	}

	const Message NetworkLinux::ReceiveMessage()
	{
		char buffer[recvBufferLength];
		fill(buffer, buffer + recvBufferLength, 0);
		sockaddr_in sender;
		int senderLen = sizeof(sender);

		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(linuxSocket, &read_fds);

		timeval time;
		time.tv_sec = 0;
		time.tv_usec = 16667; // microseconds, so this is ~1/60 sec

		int selectResult = select(linuxSocket + 1, &read_fds, nullptr, nullptr, &time);
		if (selectResult == -1)
		{
			Display_PError("select");
		}
		else if (selectResult > 0) // 0 means it timed-out
		{
			int receivedBytes = recvfrom(linuxSocket, buffer, recvBufferLength,
					0, reinterpret_cast<sockaddr*>(&sender),
					reinterpret_cast<socklen_t*>(&senderLen));

			if (receivedBytes == -1)
			{
				Display_PError("recvfrom");
			}
			else if (receivedBytes > 0)
			{
				Message msg;
				msg.prefix = buffer[0];
				msg.msg = string(buffer + 1, buffer + receivedBytes);
				msg.address = sender.sin_addr;
				return msg;
			}
		}
		Message m;
		m.prefix = 'N';
		return m;
	}

	bool NetworkLinux::StartUp()
	{
		// zero addr structures
		memset(&machineAddr, 0, sizeof machineAddr);
		memset(&broadcastAddr, 0, sizeof broadcastAddr);

		// get this machine's IP and store it
		machineAddr.sin_family = AF_INET;
		machineAddr.sin_port = htons(portNumber);
		inet_pton(AF_INET, GetIP().c_str(), &(machineAddr.sin_addr));

		// get the netmask and calculate/store the correct broadcast address
		broadcastAddr.sin_family = AF_INET;
		broadcastAddr.sin_port = htons(portNumber);
		GetNetMask();
		broadcastAddr.sin_addr.s_addr = machineAddr.sin_addr.s_addr	| ~netmask;

		char bufIP[INET_ADDRSTRLEN], bufBroadcast[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &machineAddr.sin_addr, bufIP, INET_ADDRSTRLEN);
		inet_ntop(AF_INET, &broadcastAddr.sin_addr, bufBroadcast,
				INET_ADDRSTRLEN);
		Log("IP is: " + string(bufIP) + "\nBroadcast address is: "
				+ string(bufBroadcast));

		// create socket
		linuxSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (linuxSocket == -1)
		{
			Display_PError("socket");
			return false;
		}
		Log("Socket created.");

		// switch to broadcast mode
		int broadcast = 1;
		if (setsockopt(linuxSocket, SOL_SOCKET, SO_BROADCAST, &broadcast,
				sizeof broadcast) == -1)
		{
			Display_PError("setsockopt");
			close(linuxSocket);
			return false;
		}
		Log("Socket switched to broadcast mode.");

		// turn on reuse address
		int reuse = 1;
		if (setsockopt(linuxSocket, SOL_SOCKET, SO_REUSEADDR, &reuse,
				sizeof reuse) == -1)
		{
			Display_PError("setsockopt");
			close(linuxSocket);
			return false;
		}
		Log("Reuseaddr option enabled.");

		// bind it (this simplifies things by making sure everyone is using the same port)
		sockaddr_in bindAddr;
		bindAddr.sin_family = AF_INET;
		bindAddr.sin_port = htons(portNumber);
		bindAddr.sin_addr.s_addr = INADDR_ANY;
		if (bind(linuxSocket, reinterpret_cast<sockaddr*>(&bindAddr),
				sizeof(bindAddr)) == -1)
		{
			Display_PError("bind");
			close(linuxSocket);
			return false;
		}
		Log("Socket bound.");

		return true;
	}
}

#endif
