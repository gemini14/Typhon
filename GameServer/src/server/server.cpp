#include "server/server.h"

#include <boost/foreach.hpp>

#include "utility/utility.h"

using namespace std;

namespace Typhon
{
	void Server::Acknowledge(const Message& m)
	{
		// Log("Dummy message acknowledged.");
		switch(m.msg[0])
		{
		case 'l':
			if(levelmanager != nullptr)
			{
				levelmanager->PushMessage(m);				
			}
			break;

		default:
			break;
		}
	}

	void Server::Connect(const Message& m)
	{
		auto iter = players.find(GetNetworkIP(m.address));
		// Case where unknown player from another game using ENet
		// connects ignored on purpose (unlikely, plus this isn't
		// going to be a commercial game).  If required, a simple
		// peer_reset should do the job if player not found in
		// player list (to free up space for valid players)
		if(iter != players.end())
		{
			iter->second.SetConnected(true);
			Log(iter->second.GetPlayerName() + string(" connected."));
			
			// check if everyone's in-game yet and set appropriate var
			allConnected = true;
			for(auto i = players.begin(); i != players.end(); ++i)
			{
				if(!i->second.GetConnected())
				{
					allConnected = false;
					break;
				}
			}
		}
	}
	
	void Server::Disconnect(const Message& m)
	{
		auto iter = players.find(GetNetworkIP(m.address));
		if(iter != players.end())
		{
			iter->second.SetConnected(false);
			Log(iter->second.GetPlayerName() + string(" disconnected."));
			if(network->GetIP() == GetNetworkIP(m.address))
			{
				hostLeftGame = true;
			}
		}
	}

	Server::Server(Network *net, PlayerMap players)
		: hostLeftGame(false), network(net), players(players),
		allConnected(false), levelmanager(nullptr)
	{
		callbacks['A'] = &Server::Acknowledge;
		callbacks['C'] = &Server::Connect;
		callbacks['D'] = &Server::Disconnect;
	}

	Server::~Server()
	{
		delete network;
	}

	bool Server::Run()
	{
		// handle inc message
		auto message = network->ReceiveMessage();
		if(message.prefix != 'N')
		{
			// need to dereference the stored member function pointer and call
			// it using "this", all wrapped up in parentheses
			(this->*callbacks[message.prefix])(message);
		}

		// host left, so alert players and shut down
		if(hostLeftGame)
		{
			Log("Host left game.");
			// TODO send disconnect message to all players
			return false;
		}

		if(allConnected)
		{
			if(!levelmanager)
			{
				levelmanager = unique_ptr<LevelManager>(new LevelManager(network, &players));
			}
			else
			{
				return levelmanager->Run(allConnected);
			}
		}

		return true;
	}
}
