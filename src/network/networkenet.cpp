#include "network/networkenet.h"

using namespace std;

namespace Typhon
{
	void NetworkENet::DisplayError(const std::string &message)
	{

	}

	NetworkENet::NetworkENet(const int port)
		: Network(port)
	{

	}

	NetworkENet::~NetworkENet()
	{

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