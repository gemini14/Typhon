#ifndef NETWORKENET_H
#define NETWORKENET_H

#include "network/network.h"

namespace Typhon
{
	class NetworkENet : public Network
	{
	private:

	protected:

		virtual void DisplayError(const std::string &message);

	public:

		NetworkENet(const int port);
		virtual ~NetworkENet();

		virtual void BroadcastMessage(const std::string &msg, const char prefix);
		virtual const Message ReceiveMessage();
		virtual bool StartUp();
	};
}

#endif