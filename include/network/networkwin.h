#ifndef NETWORKWIN_H
#define NETWORKWIN_H

#ifdef WIN32

#include <WinSock2.h>
#include <ws2tcpip.h>

#include "network/networkraw.h"

namespace Typhon
{
	class NetworkWin : public NetworkRaw
	{
	private:

		SOCKET winsocket;

		virtual std::string GetIP();
		virtual std::string GetNetMask();

	protected:

		virtual void DisplayError(const std::string &message);

	public:

		NetworkWin(const int port);
		virtual ~NetworkWin();

		virtual void BroadcastMessage(const std::string &msg, const char prefix);
		virtual const Message ReceiveMessage();
		virtual bool StartUp();
	};
}

#endif

#endif
