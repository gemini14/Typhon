#ifndef NETWORKRAW_H
#define NETWORKRAW_H

#include <boost/noncopyable.hpp>

#include "network/network.h"

namespace Typhon
{
	class NetworkRaw : public Network
	{
	private:

		virtual std::string GetIP() = 0;
		virtual std::string GetNetMask() = 0;

	protected:

		virtual void DisplayError(const std::string &message) = 0;

	public:

		NetworkRaw(const int port);
		virtual ~NetworkRaw();

		virtual void BroadcastMessage(const std::string &msg, const char prefix) = 0;
		virtual const Message ReceiveMessage() = 0;
		virtual bool StartUp() = 0;
	};
}

#endif