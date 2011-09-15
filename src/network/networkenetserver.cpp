#include "network/networkenetserver.h"

#include <algorithm>
#include <iostream>
#include <string>

#include <boost/thread.hpp>

#include "utility/constants.h"
#include "utility/utility.h"

using namespace std;

namespace Typhon
{
	enum ENet_Channel 
	{
		SEND_CH = 0 
	};

	boost::mutex coutMutex;

	void NetworkENetServer::DisplayError(const std::string &message)
	{
		boost::lock_guard<boost::mutex> lock(coutMutex);
		cout << message << "\n";
	}

	NetworkENetServer::NetworkENetServer(const int port, const sockaddr_in *serverIP)
		: Network(port)
	{
		copy(serverIP, serverIP + sizeof serverIP, &IP);
	}

	NetworkENetServer::~NetworkENetServer()
	{
		enet_host_destroy(server);
		enet_deinitialize();
	}

	void NetworkENetServer::BroadcastMessage(const std::string &msg, const char prefix)
	{
		string packetMsg = prefix + msg;
		ENetPacket *packet = enet_packet_create(packetMsg.c_str(), packetMsg.length(), ENET_PACKET_FLAG_RELIABLE);
		enet_host_broadcast(server, SEND_CH, packet);
		enet_host_flush(server);
	}

	const Message NetworkENetServer::ReceiveMessage()
	{
		ENetEvent event;
		if(enet_host_service(server, &event, 0))
		{
			switch(event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				{
					boost::lock_guard<boost::mutex> lock(coutMutex);
					cout << "Client connected from " << event.peer->address.host << ".\n";
				}
				// TODO perform client addition here
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				// must include at least prefix and 1 char msg
				if(event.packet->dataLength >= 2)
				{
					Message m;
					m.address = StoreIPNumber(event.peer->address.host);
					m.prefix = event.packet->data[0];
					m.msg = string(event.packet->data + 1, event.packet->data + event.packet->dataLength);

					enet_packet_destroy(event.packet);
					return m;
				}
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				{
					boost::lock_guard<boost::mutex> lock(coutMutex);
					cout << "Client at " << event.peer->address.host << " disconnected.\n";
				}
				// TODO perform client removal here
				break;
			}
		}

		Message m;
		m.prefix = 'N';
		return m;
	}

	bool NetworkENetServer::StartUp()
	{
		if(enet_initialize() != 0)
		{
			return false;
		}

		ENetAddress address;
		char IP_str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(IP.sin_addr), IP_str, INET_ADDRSTRLEN);
		enet_address_set_host(&address, IP_str);
		address.port = portNumber;

		server = enet_host_create(&address, MAX_PLAYERS, 0, 0, 0);
		if(!server)
		{
			DisplayError("ENet was not able to create a server host.");
			return false;
		}

		return true;
	}
}