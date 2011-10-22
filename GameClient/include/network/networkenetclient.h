#ifndef NETWORKENETCLIENT_H
#define NETWORKENETCLIENT_H

#include "enet/enet.h"
#include "network/network.h"

namespace Typhon
{
	class NetworkENetClient : public Network
	{
	private:

		ENetHost *client;
		sockaddr_in serverIP;

	protected:

		virtual void DisplayError(const std::string &message);

	public:

		NetworkENetClient(const int port, const sockaddr_in *IP);
		virtual ~NetworkENetClient();

		// Important: this only sends the message to the server, it does
		// not perform a UDP broadcast
		virtual void BroadcastMessage(const std::string &msg, const char prefix);
		virtual const Message ReceiveMessage();
		virtual bool StartUp();
	};
}

#endif
