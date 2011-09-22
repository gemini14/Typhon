#include "server/server.h"


namespace Typhon
{
	bool Server::hostLeftGame = false;


	void Server::HostLeftGame()
	{
		hostLeftGame = true;
	}

	void Server::ServerThreadRun(const sockaddr_in &serverIP)
	{
		Server gameServer(GetNetwork(ENETSERVER, PORT_NUMBER, &serverIP));
		if(!gameServer.gameServer)
		{
			return;
		}

		while(true)
		{


			if(hostLeftGame)
			{
			// perform clean up operations here
			hostLeftGame = false;
			}
		}
	}

	Server::Server(Network *network)
		: gameServer(network)
	{
	}

	Server::~Server()
	{
	}

	void Server::Run()
	{

	}
}
