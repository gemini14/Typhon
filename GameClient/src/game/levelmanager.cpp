#include "game/levelmanager.h"

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

using namespace boost;
using namespace irr;
using namespace std;

namespace Typhon
{
	void LevelManager::LoadLevel(const Message& m)
	{
		char_separator<char> sep("=");
		typedef tokenizer<boost::char_separator<char>> PlayerIDTokenizer;
		string arg(m.msg.begin() + m.msg.find_first_of('+') + 1, m.msg.end());
		PlayerIDTokenizer tokens(arg, sep);
		
		bool IDisNext = false;
		int playerStartID;
		for(PlayerIDTokenizer::iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
		{
			if(IDisNext)
			{
				playerStartID = lexical_cast<unsigned long>(*iter);
				break;
			}
			if(lexical_cast<unsigned long>(*iter) == network->GetIP())
			{
				IDisNext = true;
			}
		}

		Log("Level load message received, loading level...");
		string levelFile("levels/racetrackLevel");
		levelFile.append(m.msg.begin() + 1, m.msg.begin() + 2);
		levelFile.append(".irr");
		engine->smgr->loadScene(levelFile.c_str());
		auto carNode = engine->smgr->getSceneNodeFromName(string("start" +
			lexical_cast<string>(playerStartID)).c_str());
		camera = engine->smgr->addCameraSceneNode(carNode, core::vector3df(0, 0, 0), 
			core::vector3df(0, 0, 1));
		// Z offset may need to be adjusted in future maps, but there's only 1 map for now
		camera->setTarget(core::vector3df(carNode->getAbsolutePosition().X, carNode->getAbsolutePosition().Y, carNode->getAbsolutePosition().Z - 40));
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