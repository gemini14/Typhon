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
		typedef void (Server::*ServerMemFuncPtr)(const Message&);
		typedef std::unordered_map<char, ServerMemFuncPtr> CallbackMap;

		bool hostLeftGame;
		Network *gameServer;
		PlayerMap players;
		CallbackMap callbacks;
		bool allConnected;

		void Connect(const Message& m);
		void Disconnect(const Message& m);

	public:

		Server(Network *net, PlayerMap players);
		~Server();

		bool Run();			
	};
}

#endif