#include "server/server.h"

#include <boost/thread.hpp>

#include <iostream>

namespace Typhon
{
	bool Server::clientLeftGame = false;
	boost::mutex clientLeftMutex;

	Server::Server()
	{
	}

	Server::~Server()
	{
	}

	void Server::ClientLeftGame()
	{
		boost::lock_guard<boost::mutex> lockVar(clientLeftMutex);
		clientLeftGame = true;
	}

	void Server::ServerThreadRun()
	{
		while(true)
		{
			

			boost::lock_guard<boost::mutex> lockVar(clientLeftMutex);
			if(clientLeftGame)
			{
				clientLeftGame = false;
				break;
			}
		}
	}
}