#include "network/networkenetserver.h"

using namespace std;

namespace Typhon
{
	void NetworkENetServer::DisplayError(const std::string &message)
	{
	}

	NetworkENetServer::NetworkENetServer(const int port)
		: Network(port)
	{
	}

	NetworkENetServer::~NetworkENetServer()
	{
		enet_deinitialize();
	}

	void NetworkENetServer::BroadcastMessage(const std::string &msg, const char prefix)
	{
	}

	const Message NetworkENetServer::ReceiveMessage()
	{


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
		return true;
	}
}