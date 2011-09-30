#ifndef SERVER_H
#define SERVER_H

#include <unordered_map>

#include "network/networkfactory.h"
#include "server/user.h"

namespace Typhon
{
	class Server
	{
	private:

		typedef std::unordered_map<unsigned long, User> PlayerMap;
		bool hostLeftGame;
		Network *gameServer;
		PlayerMap players;

	public:

		Server(Network *net, PlayerMap players);
		~Server();

		void Run();			
	};
}

#endif