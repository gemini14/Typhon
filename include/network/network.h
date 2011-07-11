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
		static int portNumber;
		// our IP
		static sockaddr_in machineAddr;
		// broadcast IP
		static sockaddr_in broadcastAddr;

		NETWORK_HANDLING_TYPE type;

		std::string GetIP();
		std::string GetNetMask();

	public:

		Network(NETWORK_HANDLING_TYPE type, const int port);
		~Network();

		void BroadcastMessage(const std::string &msg);
		const Message ReceiveMessage();
		bool StartUp();
	};
}

#endif