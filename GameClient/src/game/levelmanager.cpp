#include "game/levelmanager.h"

#include <string>

#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace std;

namespace Typhon
{
	void LevelManager::LoadLevel(const Message& m)
	{
		string levelName("racetrackLevel" + m.msg[1]);
		levelName += ".irr";
		engine->smgr->loadScene(levelName.c_str());
		// do bullet stuff
		network->BroadcastMessage("l", 'A');
	}

	LevelManager::LevelManager(std::shared_ptr<Engine> engine)
		: engine(engine)
	{
		callbacks['L'] = &LevelManager::LoadLevel;
	}

	LevelManager::~LevelManager()
	{
	}

	void LevelManager::PushMessage(const Message& m)
	{
		messageQueue.push(m);
	}

	void LevelManager::Run()
	{
		// clear out the message queue
		while(!messageQueue.empty())
		{
			// we know prefix is 'L', use first char of message as secondary prefix
			(this->*callbacks[messageQueue.front().msg[0]])(messageQueue.front());
			messageQueue.pop();
		}

		// step physics simulation
		// send updates to players
	}
}