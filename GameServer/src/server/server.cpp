#include "server/server.h"


namespace Typhon
{
	Server::Server(Network *net, PlayerMap players)
		: hostLeftGame(false), gameServer(net), players(players)
	{
	}

	Server::~Server()
	{
		delete gameServer;
	}

	void Server::Run()
	{

	}
}