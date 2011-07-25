#include "network/networklinux.h"

using namespace std;

namespace Typhon
{
		void NetworkLinux::DisplayError(const std::string &message)
	{
		
	}

	std::string NetworkLinux::GetIP()
	{
		string ipaddr;

		return ipaddr;
	}

	std::string NetworkLinux::GetNetMask()
	{
		string mask;

		return mask;
	}

	NetworkLinux::NetworkLinux(const int port)
		: NetworkRaw(port)
	{

	}

	NetworkLinux::~NetworkLinux()
	{
		
	}

	void NetworkLinux::BroadcastMessage(const std::string &msg, const char prefix)
	{
	}

	const Message NetworkLinux::ReceiveMessage()
	{
		
		
		Message m;
		m.prefix = 'N';
		return m;
	}

	bool NetworkLinux::StartUp()
	{
		return true;
	}
}