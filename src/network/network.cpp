#include "network/network.h"

#include <iostream>

namespace Typhon
{
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

	Network::Network(NETWORK_HANDLING_TYPE type,const int port)
		: type(type)
	{
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

		GetIP();
		return true;
	}
}