#include "server/server.h"

#include <boost/thread.hpp>

#include <iostream>

namespace Typhon
{
	bool Server::hostLeftGame = false;
	boost::mutex hostLeftMutex;

	Server::Server()
	{
	}

	Server::~Server()
	{
	}

	void Server::ClientLeftGame()
	{
		boost::lock_guard<boost::mutex> lockVar(hostLeftMutex);
		hostLeftGame = true;
	}

	void Server::ServerThreadRun()
	{
		while(true)
		{
			

			boost::lock_guard<boost::mutex> lockVar(hostLeftMutex);
			if(hostLeftGame)
			{
				hostLeftGame = false;
				break;
			}
		}
	}
}