#include "game/levelmanager.h"

#include <string>

#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace std;

namespace Typhon
{
	void LevelManager::LoadLevel(const Message& m)
	{
		Log("Level load message received, loading level...");
		string levelFile("levels/racetrackLevel");
		levelFile.append(m.msg.begin() + 1, m.msg.end());
		levelFile.append(".irr");
		engine->smgr->loadScene(levelFile.c_str());
		auto carNode = engine->smgr->getSceneNodeFromName("start1");
		camera = engine->smgr->addCameraSceneNode(carNode);
		// do bullet stuff
		network->BroadcastMessage("l", 'A');
		Log("Level load complete, acknowledgement sent.");
	}

	LevelManager::LevelManager(std::shared_ptr<Engine> engine)
		: engine(engine), camera(nullptr)
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