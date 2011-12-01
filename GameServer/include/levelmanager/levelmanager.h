#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <bitset>
#include <memory>
#include <unordered_map>
#include <queue>

#include "utility/constants.h"
#include "network/network.h"
#include "physics/physics.h"
#include "server/user.h"

namespace Typhon
{

class LevelManager
{
private:

	typedef std::unordered_map<unsigned long, User> PlayerMap;
	typedef void (LevelManager::*LevelManagerMemFuncPtr)(const Message&);
	typedef std::unordered_map<char, LevelManagerMemFuncPtr> CallbackMap;

	bool gameOver;
	int numLevels;
	int chosenLevel;
	bool allPlayersLoaded;

	Network *network;
	PlayerMap *players;
	std::bitset<MAX_PLAYERS> loadedLevel;
	std::unique_ptr<Physics> physics;
	CallbackMap callbacks;
	std::queue<Message> messageQueue;

	void AcknowledgeLoad(const Message& m);

public:

	LevelManager(Network *network, PlayerMap *players);
	~LevelManager();

	void PlayerLoadComplete(const int playerID);
	void PushMessage(const Message& m);
	bool Run(const bool allConnected);
};

}

#endif
