#ifndef NETWORKLINUX_H
#define NETWORKLINUX_H

#include "network/networkraw.h"

namespace Typhon
{
	class NetworkLinux : public NetworkRaw
	{
	private:

		virtual std::string GetIP();
		virtual std::string GetNetMask();

	protected:

		virtual void DisplayError(const std::string &message);

	public:

		NetworkLinux(const int port);
		virtual ~NetworkLinux();

		virtual void BroadcastMessage(const std::string &msg, const char prefix);
		virtual const Message ReceiveMessage();
		virtual bool StartUp();
	};
}

#endif