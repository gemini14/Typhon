#include "network/networkenetclient.h"

#include <boost/lexical_cast.hpp>

#include "utility/utility.h"

using namespace std;

namespace Typhon
{
	void NetworkENetClient::DisplayError(const std::string &message)
	{
		Log(message);
	}

	NetworkENetClient::NetworkENetClient(const int port, const sockaddr_in *IP)
		: Network(port), client(nullptr), server(nullptr), serverIP(*IP)
	{
		serverIP.sin_family = AF_INET;
#ifdef WIN32
		serverIP.sin_addr.S_un.S_addr = IP->sin_addr.S_un.S_addr;
#else
		serverIP.sin_addr.s_addr = IP->sin_addr.s_addr;
#endif
	}

	NetworkENetClient::~NetworkENetClient()
	{
		if(client)
		{
			ENetEvent event;
			enet_peer_disconnect(server, 0);
			bool disconnected = false;
			int disconnectTries = 0;
			while(!disconnected && disconnectTries < 5 &&
				enet_host_service(client, &event, 1000))
			{
				switch(event.type)
				{
				case ENET_EVENT_TYPE_RECEIVE:
					enet_packet_destroy(event.packet);
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					Log("Disconnect from server successful.");
					disconnected = true;
					break;
				}
				++disconnectTries;
			}
			if(!disconnected)
			{
				enet_peer_reset(server);
			}

			enet_host_destroy(client);
		}
		enet_deinitialize();
	}

	void NetworkENetClient::BroadcastMessage(const std::string &msg, const char prefix)
	{
		string packetMsg = prefix + msg;
		ENetPacket *packet = enet_packet_create(packetMsg.c_str(), packetMsg.length(), ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(server, 0, packet);
		enet_host_flush(client);
	}

	bool NetworkENetClient::ConnectToServer()
	{
		ENetAddress address;
		char buffer[INET_ADDRSTRLEN];
		Log("Server IP is " + string(inet_ntop(AF_INET, &(serverIP.sin_addr), buffer, INET_ADDRSTRLEN)));
		enet_address_set_host(&address, buffer);
		// connect to the set port number (1550) + 1 to avoid conflicts with previous
		// socket operations on the same port in the client's (who's hosting the server)
		// lobby operations
		address.port = portNumber + 1;

		server = enet_host_connect(client, &address, 2, 0);
		if(!server)
		{
			DisplayError("No available server for initiating ENet connection.");
			return false;
		}
		Log("Server peer object created.");

		ENetEvent event;

		const int maxAttempts = 3;
		int tries = 0;
		while(tries < maxAttempts)
		{
			Log("Waiting for connection acknowledgement...try #" + boost::lexical_cast<string>(tries + 1));
			if(enet_host_service(client, &event, 5000) && event.type == ENET_EVENT_TYPE_CONNECT)
			{
				Log("Connected to server successfully!");
				// for some reason, client needs to send a message to get server
				// to generate connection event
				BroadcastMessage("", 'A');
				break;
			}
			else
			{
				++tries;
			}
		}

		if(tries == maxAttempts)
		{
			// timed out or disconnect event received
			enet_peer_reset(server);
			Log("Failed to connect to server.");
			return false;
		}

		return true;
	}

	void NetworkENetClient::DisconnectFromServer()
	{
		if(server)
		{
			enet_peer_disconnect(server, 0);
			bool disconnectedCleanly = false;
			ENetEvent event;
			// wait 5 seconds for disconnect
			while(!disconnectedCleanly && enet_host_service(client, &event, 5000) > 0)
			{
				switch(event.type)
				{
				case ENET_EVENT_TYPE_RECEIVE:
					enet_packet_destroy(event.packet);
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					Log("Disconnected cleanly from server.");
					disconnectedCleanly = true;
					break;
				}
			}
			if(!disconnectedCleanly)
			{
				enet_peer_reset(server);
			}
		}
	}

	const Message NetworkENetClient::ReceiveMessage()
	{
		ENetEvent event;
		Message m;
		m.prefix = 'N';

		if(enet_host_service(client, &event, 10))
		{
			switch(event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				// must include at least prefix and 1 char msg
				if(event.packet->dataLength >= 1)
				{
					Message m;
					m.address = StoreIPNumber(event.peer->address.host);
					m.prefix = event.packet->data[0];
					if(event.packet->dataLength > 1)
					{
						m.msg = string(reinterpret_cast<const char*>(event.packet->data + 1), event.packet->dataLength - 1);
					}

					enet_packet_destroy(event.packet);
					return m;
				}
				else
				{
					enet_packet_destroy(event.packet);
				}
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				Log("Server at " + boost::lexical_cast<string>(event.peer->address.host) + " disconnected.");
				m.prefix = 'D';
				m.address = StoreIPNumber(event.peer->address.host);
				return m;
			}
		}

		return m;
	}

	bool NetworkENetClient::StartUp()
	{
		if(enet_initialize() != 0)
		{
			return false;
		}

		client = enet_host_create(nullptr, 1, 2, 0, 0);

		if(!client)
		{
			DisplayError("ENet was not able to create a client.");
			return false;
		}
		Log("Client created.");

		return true;
	}
}
