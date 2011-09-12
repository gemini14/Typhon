#include "network/networkenetclient.h"

using namespace std;

namespace Typhon
{
	void NetworkENetClient::DisplayError(const std::string &message)
	{
	}

	NetworkENetClient::NetworkENetClient(const int port, const sockaddr_in *IP)
		: Network(port), serverIP(*IP)
	{
	}

	NetworkENetClient::~NetworkENetClient()
	{
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
		return true;
	}
}