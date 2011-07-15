#include "network/network.h"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace Typhon
{
	const int recvBufferLength = 1024;
	int Network::portNumber;

	sockaddr_in Network::machineAddr;
	sockaddr_in Network::broadcastAddr;

	void Network::DisplayError(const std::string &message)
	{
		std::cout << message << " Error: " << WSAGetLastError() << "\n";
	}

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
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;
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
			DisplayError("Socket creation failed.");
			return "";
		}

		// 20 interfaces should easily cover most home network setups
		INTERFACE_INFO interfaceList[20];
		unsigned long bytesReturned;
		if(WSAIoctl(sock, SIO_GET_INTERFACE_LIST, 0, 0, &interfaceList, sizeof(interfaceList),
			&bytesReturned, 0, 0) == SOCKET_ERROR)
		{
			DisplayError("Winsock function (WSAIoctl) failed.");
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
#ifdef WIN32
		if(type == RAW)
		{
			winsocket = INVALID_SOCKET;
		}
#else
#endif
	}

	Network::~Network()
	{
#ifdef WIN32
		WSACleanup();
#else
#endif
	}

	void Network::BroadcastMessage(const std::string &msg, const char prefix)
	{
#ifdef WIN32

		// Lambda to continue sending until delivery of entire message is complete
		auto completeSend = [=](const std::string &msg) -> int
		{
			int totalSent = 0, bytesLeft = static_cast<int>(msg.length()), bytesSent = 0;
			while(totalSent < bytesLeft)
			{
				bytesSent = sendto(winsocket, msg.c_str() + totalSent, bytesLeft, 0,
					reinterpret_cast<sockaddr*>(&broadcastAddr), sizeof(broadcastAddr));

				if(bytesSent == -1) // SOCKET_ERROR = -1
				{
					break;
				}

				totalSent += bytesSent;
				bytesLeft -= bytesSent;
			}

			// return -1 to indicate a socket error occurred while trying to send
			return (bytesSent == -1) ? -1 : 0;
		};

		// WinSock
		if(type == RAW)
		{
			std::stringstream dataStream;
			dataStream << prefix << '%' << msg.length() << '%' << msg;
			std::cout << "Getting ready to send data (" << dataStream.str() << ").\n";
			if(completeSend(dataStream.str()) == -1)
			{
				DisplayError("Failure sending the complete message (socket error, raw).");
			}
			else
			{
				std::cout << "Message sent.\n";
			}
		}
		// ENet
		else
		{
		}
#else
		if(type == RAW)
		{
		}
		else
		{
		}
#endif
	}

	const Message Network::ReceiveMessage()
	{
#ifdef WIN32
		// WinSock
		if(type == RAW)
		{
			char buffer[recvBufferLength];
			std::fill(buffer, buffer + recvBufferLength, 0);
			sockaddr_in sender;
			int senderLen = sizeof(sender);

			fd_set read_fds;
			FD_ZERO(&read_fds);
			FD_SET(winsocket, &read_fds);
			
			timeval time;
			time.tv_sec = 0;
			time.tv_usec = 1000000; // microseconds, so this is 500 ms or 0.5 sec

			int selectResult = select(0, &read_fds, nullptr, nullptr, &time);
			if(selectResult == SOCKET_ERROR)
			{
				DisplayError("Error checking for readability with select().");
			}
			else if(selectResult > 0) // 0 means it just timed out
			{
				int receivedBytes = recvfrom(winsocket, buffer, recvBufferLength, 0,
					reinterpret_cast<sockaddr*>(&sender), &senderLen);
				std::cout << "Received " << receivedBytes << " bytes from sender.\n";
				Message msg;
				if(receivedBytes)
				{				
					msg.msg = buffer;
					msg.prefix = buffer[0];
					msg.address = sender.sin_addr;
				}
				else
				{
					msg.prefix = 'N';
				}
				return msg;
			}
		}		
		// ENet
		else
		{
		}
#else
#endif
		Message m;
		m.prefix = 'N';
		return m;
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
				DisplayError("Winsock function (WSAStartup) failed.");
				return false;
			}

			// zero addr structures
			memset(&machineAddr, 0, sizeof(machineAddr));
			memset(&broadcastAddr, 0, sizeof(broadcastAddr));

			// get this machine's IP and store it
			machineAddr.sin_family = AF_INET;
			machineAddr.sin_port = htons(portNumber);
			inet_pton(AF_INET, GetIP().c_str(), &(machineAddr.sin_addr));

			// get the netmask and calculate/store the correct broadcast address
			broadcastAddr.sin_family = AF_INET;
			broadcastAddr.sin_port = htons(portNumber);
			struct in_addr netmask;
			memset(&netmask, 0, sizeof(netmask));
			inet_pton(AF_INET, GetNetMask().c_str(), &netmask);
			broadcastAddr.sin_addr.S_un.S_addr = machineAddr.sin_addr.S_un.S_addr | (~netmask.S_un.S_addr);

			char bufIP[INET_ADDRSTRLEN], bufBroadcast[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &machineAddr.sin_addr, bufIP, INET_ADDRSTRLEN);
			inet_ntop(AF_INET, &broadcastAddr.sin_addr, bufBroadcast, INET_ADDRSTRLEN);
			std::cout << "IP is: " << bufIP << "\nBroadcast address is: " << bufBroadcast << "\n";

			// winsock started up OK, now create socket
			winsocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if(winsocket == INVALID_SOCKET)
			{
				DisplayError("Winsock failed to create socket.");
				return false;
			}
			std::cout << "Socket created.\n";

			// switch it to broadcast mode
			char optionSet = '1';
			if(setsockopt(winsocket, SOL_SOCKET, SO_BROADCAST, &optionSet, sizeof(int)) == SOCKET_ERROR)
			{
				DisplayError("Failed to switch socket to broadcast mode.");
				closesocket(winsocket);
				return false;
			}
			std::cout << "Socket switched to broadcast mode.\n";

			// bind it (this simplifies things by making sure everyone is using the same port)
			if(bind(winsocket, reinterpret_cast<sockaddr*>(&machineAddr), sizeof(machineAddr)) == SOCKET_ERROR)
			{
				DisplayError("Socket binding failed.");
				closesocket(winsocket);
				return false;
			}
			std::cout << "Socket bound.\n";
		}
		// ENet
		else
		{
		}
#else
		// Linux raw
		if(type == RAW)
		{
		}
		else
		{
		}
#endif
		return true;
	}
}