#include "levelmanager/levelmanager.h"

#include <string>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "utility/constants.h"
#include "utility/utility.h"

using namespace boost;
using namespace std;

namespace Typhon
{
	void LevelManager::AcknowledgeLoad(const Message& m)
	{
		int i = 0;
		for(auto iter = players->begin();  ; ++i, ++iter)
		{
			if(iter->first == GetNetworkIP(m.address))
			{
				// mark player as having completely loaded their level
				loadedLevel[i] = true;
			}
		}
	}

	LevelManager::LevelManager(Network *network, PlayerMap *players) :
	gameOver(false), numLevels(1), chosenLevel(0),
		allPlayersLoaded(false), network(network),
		players(players), loadedLevel(MAX_PLAYERS), physics(new Physics())
	{
		callbacks['A'] = &LevelManager::AcknowledgeLoad;

		string playerIDs;
		int i = 0;
		for(auto iter = players->begin(); iter != players->end(); ++iter, ++i)
		{
			if(iter->second.GetType() == HUMAN)
			{
				playerIDs += lexical_cast<string>(iter->first) + string("=") +
					lexical_cast<string>(iter->second.GetID()) + string("=");
			}
			// bots are already "connected"
			if(iter->second.GetType() == AI)
			{
				loadedLevel[iter->second.GetID()] = true;
			}
		}
		
		// this could be changed to load a random level if more are added later
		Log("Sending level load message.");
		network->BroadcastMessage("L1+" + playerIDs, 'L');

		// TODO load level mesh, etc. stuff into Bullet
	}

	LevelManager::~LevelManager()
	{

	}

	void LevelManager::PlayerLoadComplete(const int playerID)
	{
		loadedLevel[playerID] = 1;
	}

	void LevelManager::PushMessage(const Message& m)
	{
		messageQueue.push(m);
	}

	bool LevelManager::Run(const bool allConnected)
	{
		if(!allPlayersLoaded)
		{
			for(size_t i = loadedLevel.size(); i >= 0; --i)
			{
				if(!loadedLevel[i])
				{
					// not everyone is ready yet, so just return until they are
					return true;
				}
			}
			// if we made it here, everyone's good to go, so note it
			allPlayersLoaded = true;
		}

		// clear out the message queue
		while(!messageQueue.empty())
		{
			(this->*callbacks[messageQueue.front().prefix])(messageQueue.front());
			messageQueue.pop();
		}

		// step physics simulation
		// send updates to players
		// send/recv/handle powerup stuff
		// note if anyone crosses finish line

		if (gameOver)
		{
			// indicate that everything's been played and server should shut down
			return false;
		}

		return true;
	}
}