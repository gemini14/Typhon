#include "server/server.h"

#include <boost/thread.hpp>

namespace Typhon
{
	bool Server::hostLeftGame = false;
	boost::mutex hostLeftMutex;

	void Server::HostLeftGame()
	{
		boost::lock_guard<boost::mutex> lockVar(hostLeftMutex);
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


			{
				boost::lock_guard<boost::mutex> lockVar(hostLeftMutex);
				if(hostLeftGame)
				{
					// perform clean up operations here

					hostLeftGame = false;
					break;
				}
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