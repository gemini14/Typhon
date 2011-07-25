#ifndef NETWORK_H
#define NETWORK_H

#include <boost/noncopyable.hpp>
#include <string>

#ifdef WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

namespace Typhon
{
	struct Message
	{
		char prefix;
		std::string msg;
		in_addr address;
	};

	class Network : boost::noncopyable
	{
	private:

	protected:
		// designated port
		static int portNumber;
		// our IP
		static sockaddr_in machineAddr;
		// broadcast IP
		static sockaddr_in broadcastAddr;

		static const int recvBufferLength = 1024;

		virtual void DisplayError(const std::string &message) = 0;

	public:

		Network(const int port);
		virtual ~Network();

		virtual void BroadcastMessage(const std::string &msg, const char prefix) = 0;
		virtual const Message ReceiveMessage() = 0;
		virtual bool StartUp() = 0;
	};
}

#endif