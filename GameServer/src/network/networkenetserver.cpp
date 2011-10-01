#include "network/networkenetserver.h"

#include <algorithm>
#include <string>

#include <boost/lexical_cast.hpp>

#include "utility/constants.h"
#include "utility/utility.h"

using namespace std;
using namespace Typhon;

namespace Typhon
{
	void NetworkENetServer::DisplayError(const std::string &message)
	{
		Log(message);
	}

	NetworkENetServer::NetworkENetServer(const int port, const unsigned long serverIP)
		: Network(port), server(nullptr), IP(htonl(serverIP))
	{
#ifdef WIN32
		machineAddr.sin_addr.S_un.S_addr = IP;
#else
		machineAddr.sin_addr.s_addr = IP;
#endif
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
		enet_host_broadcast(server, 0, packet);
		enet_host_flush(server);
	}

	const Message NetworkENetServer::ReceiveMessage()
	{
		ENetEvent event;
		Message m;
		m.prefix = 'N';
		
		if(enet_host_service(server, &event, 0))
		{
			switch(event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				Log("Client connected from " + boost::lexical_cast<string>(event.peer->address.host));
				m.prefix = 'C';
				m.address = StoreIPNumber(event.peer->address.host);
				return m;

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
				m.prefix = 'D';
				m.address = StoreIPNumber(event.peer->address.host);
				return m;
			}
		}
		
		return m;
	}

	bool NetworkENetServer::StartUp()
	{
		if(enet_initialize() != 0)
		{
			return false;
		}
		Log("ENet initialized successfully.");

		ENetAddress address;
		char buffer[INET_ADDRSTRLEN];
		sockaddr_in addr;
#ifdef WIN32
		addr.sin_addr.S_un.S_addr = IP;
#else
		addr.sin_addr.s_addr = IP;
#endif
		Log("Server IP is: " + string(inet_ntop(AF_INET, &(addr.sin_addr), buffer, INET_ADDRSTRLEN)));
		enet_address_set_host(&address, buffer);
		address.port = portNumber;

		server = enet_host_create(&address, MAX_PLAYERS, 1, 0, 0);
		if(!server)
		{
			DisplayError("ENet was not able to create a server host.");
			return false;
		}
		Log("ENet server created.");

		return true;
	}
}
