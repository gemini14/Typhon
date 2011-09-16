#include "network/networkenetclient.h"

#include <boost/thread.hpp>

#include "utility/utility.h"

using namespace std;

namespace Typhon
{
	void NetworkENetClient::DisplayError(const std::string &message)
	{
	}

	NetworkENetClient::NetworkENetClient(const int port, const sockaddr_in *IP)
		: Network(port), client(nullptr), serverIP(*IP)
	{
	}

	NetworkENetClient::~NetworkENetClient()
	{
		enet_host_destroy(client);
		enet_deinitialize();
	}

	void NetworkENetClient::BroadcastMessage(const std::string &msg, const char prefix)
	{
	}

	const Message NetworkENetClient::ReceiveMessage()
	{


		Message m;
		m.prefix = 'N';
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

		ENetAddress address;
		enet_address_set_host(&address, GetIPStringForm(&serverIP).c_str());
		address.port = portNumber;

		ENetPeer *peer = enet_host_connect(client, &address, 2, 0);
		if(!peer)
		{
			DisplayError("No available peers for initiating ENet connection.");
			return false;
		}

		ENetEvent event;
		if(enet_host_service(client, &event, 10000) && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			cout << "Connected to server successfully!\n";
		}
		else
		{
			// timed out or disconnect event received
			enet_peer_reset(peer);
			cout << "Failed to connect to server.\n";
		}

		return true;
	}
}