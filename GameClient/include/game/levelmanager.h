#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <memory>
#include <queue>
#include <unordered_map>

#include "irrlicht/irrlicht.h"

#include "engine/engine.h"
#include "network/network.h"

namespace Typhon
{
	class LevelManager
	{
	private:

		typedef void (LevelManager::*LevelManagerMemFuncPtr)(const Message&);
		typedef std::unordered_map<char, LevelManagerMemFuncPtr> CallbackMap;

		CallbackMap callbacks;
		std::shared_ptr<Engine> engine;
		std::queue<Message> messageQueue;
		irr::scene::ICameraSceneNode *camera;

		void LoadLevel(const Message& m);

	public:

		std::shared_ptr<Network> network;

		LevelManager(std::shared_ptr<Engine> engine);
		~LevelManager();

		void PushMessage(const Message& m);
		void Run();
	};
}

#endif