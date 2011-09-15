#ifndef SERVER_H
#define SERVER_H

#include <memory>

#include "network/networkfactory.h"

namespace Typhon
{
	class Server
	{
	private:

		static bool hostLeftGame;

		std::unique_ptr<Network> gameServer;

	public:

		static void HostLeftGame();
		static void ServerThreadRun(const sockaddr_in &serverIP);

		Server(Network *network);
		~Server();

		void Run();			
	};
}

#endif