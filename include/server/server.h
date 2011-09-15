#ifndef SERVER_H
#define SERVER_H

#include "network/networkfactory.h"

namespace Typhon
{
	class Server
	{
	private:

		static bool hostLeftGame;

	public:

		Server();
		~Server();

		static void ClientLeftGame();
		static void ServerThreadRun();		
	};
}

#endif