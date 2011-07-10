#ifndef NETWORK_H
#define NETWORK_H

#ifdef WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#endif

#include <boost/noncopyable.hpp>
#include <string>

namespace Typhon
{
	enum NETWORK_HANDLING_TYPE
	{
		RAW,
		ENET
	};

	struct Message
	{
		std::string msg;
		std::string player;
	};

	class Network : boost::noncopyable
	{
	private:

		// designated port
		int portNumber;
		// our IP
		sockaddr_storage machineAddr;
		// broadcast IP
		sockaddr_storage broadcastAddr;

		NETWORK_HANDLING_TYPE type;

		std::string GetIP();

	public:

		Network(NETWORK_HANDLING_TYPE type, const int port);
		~Network();

		void BroadcastMessage(const std::string &msg);
		const Message ReceiveMessage();
		bool StartUp();
	};
}

#endif