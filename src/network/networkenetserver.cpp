#include "network/networkenetserver.h"

#include <algorithm>
#include <string>

#include <boost/lexical_cast.hpp>

#include "utility/constants.h"
#include "utility/utility.h"

using namespace std;

namespace Typhon
{
	enum ENet_Channel 
	{
		SEND_CH = 0 
	};

	void NetworkENetServer::DisplayError(const std::string &message)
	{
		Log(message);
	}

	NetworkENetServer::NetworkENetServer(const int port, const sockaddr_in *serverIP)
		: Network(port), server(nullptr)
	{
		copy(serverIP, serverIP + sizeof serverIP, &IP);
	}

	NetworkENetServer::~NetworkENetServer()
	{
		if(server)
		{
			enet_host_destroy(server);
		}
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
				Log("Client connected from " + boost::lexical_cast<string>(event.peer->address.host));

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
				Log("Client at " + boost::lexical_cast<string>(event.peer->address.host) + " disconnected.");

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
		address.host = GetNetworkIP(IP);
		address.port = portNumber;

		server = enet_host_create(&address, MAX_PLAYERS, 2, 0, 0);
		if(!server)
		{
			DisplayError("ENet was not able to create a server host.");
			return false;
		}

		return true;
	}
}