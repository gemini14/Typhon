#ifndef NETWORKENETSERVER_H
#define NETWORKENETSERVER_H

#include "enet/enet.h"
#include "network/network.h"

namespace Typhon
{
	class NetworkENetServer : public Network
	{
	private:

		ENetHost *server;
		sockaddr_in IP;

	protected:

		virtual void DisplayError(const std::string &message);

	public:

		NetworkENetServer(const int port, const sockaddr_in *serverIP);
		virtual ~NetworkENetServer();

		virtual void BroadcastMessage(const std::string &msg, const char prefix);
		virtual const Message ReceiveMessage();
		virtual bool StartUp();
	};
}

#endif