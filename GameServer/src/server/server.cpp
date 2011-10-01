#include "server/server.h"

#include <boost/foreach.hpp>

#include "utility/utility.h"

using namespace std;

namespace Typhon
{
	void Server::Connect(const Message& m)
	{
		auto iter = players.find(GetNetworkIP(m.address));
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

	}

	Server::Server(Network *net, PlayerMap players)
		: hostLeftGame(false), gameServer(net), players(players),
		allConnected(false)
	{
		callbacks['C'] = &Server::Connect;
		callbacks['D'] = &Server::Disconnect;
	}

	Server::~Server()
	{
		delete gameServer;
	}

	void Server::Run()
	{
		auto message = gameServer->ReceiveMessage();
		char prefix = message.prefix;
		if(prefix != 'N')
		{
			// need to dereference the stored member function pointer and call
			// it using "this", all wrapped up in parentheses
			(this->*callbacks[prefix])(message);
		}
	}
}