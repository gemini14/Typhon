#ifndef NETWORK_H
#define NETWORK_H

#include <boost/noncopyable.hpp>
#include <string>

#ifdef WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
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
		
		// broadcast IP
		static sockaddr_in broadcastAddr;
		// our IP
		static sockaddr_in machineAddr;
		static const int recvBufferLength = 1024;

		virtual void DisplayError(const std::string &message) = 0;
	
	public:
		
		// designated port
		static int portNumber;

		Network(const int port);
		virtual ~Network();

		virtual void BroadcastMessage(const std::string &msg, const char prefix) = 0;
		const unsigned long GetIP() const;
		std::string GetIPInStringForm() const;
		virtual const Message ReceiveMessage() = 0;
		virtual bool StartUp() = 0;
	};
}

#endif