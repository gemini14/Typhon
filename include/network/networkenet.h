#ifndef NETWORKENET_H
#define NETWORKENET_H

#include "enet/enet.h"
#include "network/network.h"

namespace Typhon
{
	class NetworkENet : public Network
	{
	private:

		sockaddr_in serverIP;

	protected:

		virtual void DisplayError(const std::string &message);

	public:

		NetworkENet(const int port, const sockaddr_in *IP);
		virtual ~NetworkENet();

		virtual void BroadcastMessage(const std::string &msg, const char prefix);
		virtual const Message ReceiveMessage();
		virtual bool StartUp();
	};
}

#endif