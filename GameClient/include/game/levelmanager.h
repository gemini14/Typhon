#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <memory>
#include <queue>
#include <unordered_map>

#include "engine/engine.h"
#include "network/message.h"

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

		void LoadLevel(const Message& m);

	public:

		LevelManager(std::shared_ptr<Engine> engine);
		~LevelManager();

		void PushMessage(const Message& m);
		void Run();
	};
}

#endif