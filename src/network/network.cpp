#include "network/network.h"

#include <iostream>

namespace Typhon
{
	int Network::portNumber;
	sockaddr_in Network::machineAddr;
	sockaddr_in Network::broadcastAddr;

	std::string Network::GetIP()
	{
		std::string ipaddr;
#ifdef WIN32
		char hostname[256];
		gethostname(hostname, 256);

		struct addrinfo *result = nullptr;
		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		
		// get IP address by using the host name (ANSI host name to address translation)
		// use IPv4 for home networks
		hints.ai_family = AF_INET;
		getaddrinfo(hostname, nullptr, &hints, &result);

		for( ; result != nullptr; result = result->ai_next)
		{
			switch(result->ai_family)
			{
			case AF_INET:
				char buffer[INET_ADDRSTRLEN];
				ipaddr += inet_ntop(AF_INET, &(reinterpret_cast<sockaddr_in*>(result->ai_addr)->sin_addr), 
					buffer, INET_ADDRSTRLEN);
				break;
			default:
				// address with another protocol (i.e, IPv6)
				break;
			}
		}

		freeaddrinfo(result);
#else
#endif
		
		return ipaddr;
	}

	std::string Network::GetNetMask()
	{
		std::string mask;

#ifdef WIN32
		SOCKET sock = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
		if(sock == SOCKET_ERROR)
		{
			std::cout << "Socket creation failed. Error: " << WSAGetLastError() << "\n";
			return "";
		}

		// 20 interfaces should easily cover most home network setups
		INTERFACE_INFO interfaceList[20];
		unsigned long bytesReturned;
		if(WSAIoctl(sock, SIO_GET_INTERFACE_LIST, 0, 0, &interfaceList, sizeof(interfaceList),
			&bytesReturned, 0, 0) == SOCKET_ERROR)
		{
			std::cout << "Winsock function (WSAIoctl) failed. Error: " << WSAGetLastError() << "\n";
			return "";
		}
		closesocket(sock);

		int numInterfaces = bytesReturned / sizeof(INTERFACE_INFO);
		char buffer[INET_ADDRSTRLEN];
		char IP[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &machineAddr.sin_addr, IP, INET_ADDRSTRLEN);
		for(int i = 0; i < numInterfaces; ++i)
		{
			sockaddr_in *address = reinterpret_cast<sockaddr_in*>(&interfaceList[i].iiAddress);
			
			if(!strcmp(inet_ntop(AF_INET, &address->sin_addr, buffer, INET_ADDRSTRLEN), IP))
			{
				mask.assign(inet_ntop(AF_INET, 
					&reinterpret_cast<sockaddr_in*>(&interfaceList[i].iiNetmask)->sin_addr,
					buffer, INET_ADDRSTRLEN));
				break;
			}
		}
#else
#endif

		return mask;
	}

	Network::Network(NETWORK_HANDLING_TYPE type,const int port)
		: type(type)
	{
		portNumber = port;
	}

	Network::~Network()
	{
#ifdef WIN32
		WSACleanup();
#else
#endif
	}

	void Network::BroadcastMessage(const std::string &msg)
	{
	}

	const Message Network::ReceiveMessage()
	{
		return Message();
	}

	bool Network::StartUp()
	{
#ifdef WIN32
		// Winsock
		if(type == RAW)
		{
			WSADATA wsaData;
			int result = WSAStartup(MAKEWORD(2,2), &wsaData);
			if(result)
			{
				std::cout << "Winsock function (WSAStartup) failed. Error: " << result << "\n";
				return false;
			}

			// zero addr structures
			memset(&machineAddr, 0, sizeof(machineAddr));
			memset(&broadcastAddr, 0, sizeof(broadcastAddr));
			
			// get this machine's IP and store it
			machineAddr.sin_family = AF_INET;
			inet_pton(AF_INET, GetIP().c_str(), &(machineAddr.sin_addr));
			
			// get the netmask and calculate/store the correct broadcast address
			broadcastAddr.sin_family = AF_INET;
			struct in_addr netmask;
			memset(&netmask, 0, sizeof(netmask));
			inet_pton(AF_INET, GetNetMask().c_str(), &netmask);
			broadcastAddr.sin_addr.S_un.S_addr = machineAddr.sin_addr.S_un.S_addr | (~netmask.S_un.S_addr);

			char bufIP[INET_ADDRSTRLEN], bufBroadcast[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &machineAddr.sin_addr, bufIP, INET_ADDRSTRLEN);
			inet_ntop(AF_INET, &broadcastAddr.sin_addr, bufBroadcast, INET_ADDRSTRLEN);
			std::cout << "IP is: " << bufIP << "\nBroadcast address is: " << bufBroadcast << "\n";

			//// winsock started up OK, now create socket
			//SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			//if(sock == INVALID_SOCKET)
			//{
			//	std::cout << "Winsock failed to create socket. Error: " << WSAGetLastError() << "\n";
			//	return false;
			//}

			//char optionSet = '1';
			//if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optionSet, sizeof(char)) == SOCKET_ERROR)
			//{
			//	std::cout << "Failed to switch socket to broadcast mode. Error: " << WSAGetLastError() << "\n";
			//	return false;
			//}
		}
		// ENet
		else
		{
		}
#else
#endif
		return true;
	}
}