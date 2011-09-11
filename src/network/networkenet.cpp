#include "network/networkenet.h"

using namespace std;

#include "utility/networkinitexception.h"

namespace Typhon
{
	void NetworkENet::DisplayError(const std::string &message)
	{

	}

	NetworkENet::NetworkENet(const int port, const sockaddr_in *IP)
		: Network(port), serverIP(*IP)
	{
		if(enet_initialize() != 0)
		{
			throw NetworkInitException("ENet failed to start.");
		}
	}

	NetworkENet::~NetworkENet()
	{
		enet_deinitialize();
	}

	void NetworkENet::BroadcastMessage(const std::string &msg, const char prefix)
	{
	}

	const Message NetworkENet::ReceiveMessage()
	{


		Message m;
		m.prefix = 'N';
		return m;
	}

	bool NetworkENet::StartUp()
	{
		return true;
	}
}