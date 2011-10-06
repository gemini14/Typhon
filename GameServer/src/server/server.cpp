#include "server/server.h"

#include <boost/foreach.hpp>

#include "utility/utility.h"

using namespace std;

namespace Typhon
{
	void Server::Acknowledge(const Message& m)
	{
		Log("Dummy message acknowledged.");
	}

	void Server::Connect(const Message& m)
	{
		auto iter = players.find(GetNetworkIP(m.address));
		// Case where unknown player from another game using ENet
		// connects ignored on purpose (unlikely, plus this isn't
		// going to be a commercial game).  If required, a simple
		// peer_reset should do the job if player not found in
		// player list
		if(iter != players.end())
		{
			iter->second.SetConnected(true);
			Log(iter->second.GetPlayerName() + string(" connected."));
			
			// check if everyone's in-game yet and set appropriate var
			bool allInGame = true;
			for(auto i = players.begin(); i != players.end(); ++i)
			{
				if(!i->second.GetConnected())
				{
					allInGame = false;
				}
			}
			allConnected = allInGame;
		}
	}
	
	void Server::Disconnect(const Message& m)
	{
		auto iter = players.find(GetNetworkIP(m.address));
		if(iter != players.end())
		{
			iter->second.SetConnected(false);
			Log(iter->second.GetPlayerName() + string(" disconnected."));
			if(gameServer->GetIP() == GetNetworkIP(m.address))
			{
				hostLeftGame = true;
			}
		}
	}

	Server::Server(Network *net, PlayerMap players)
		: hostLeftGame(false), gameServer(net), players(players),
		allConnected(false)
	{
		callbacks['A'] = &Server::Acknowledge;
		callbacks['C'] = &Server::Connect;
		callbacks['D'] = &Server::Disconnect;
	}

	Server::~Server()
	{
		delete gameServer;
	}

	bool Server::Run()
	{
		auto message = gameServer->ReceiveMessage();
		if(message.prefix != 'N')
		{
			// need to dereference the stored member function pointer and call
			// it using "this", all wrapped up in parentheses
			(this->*callbacks[message.prefix])(message);
		}

		if(hostLeftGame)
		{
			Log("Host left game.");
			return false;
		}

		return true;
	}
}