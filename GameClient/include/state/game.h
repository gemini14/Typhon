#ifndef GAME_H
#define GAME_H

#include <unordered_map>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "game/levelmanager.h"
#include "network/networkfactory.h"
#include "state/fsmstate.h"

namespace Typhon
{
	class Game : public FSMState
	{
	private:

		typedef void (Game::*ClientMemFuncPtr)(const Message&);
		typedef std::unordered_map<char, ClientMemFuncPtr> CallbackMap;

#ifdef WIN32
		STARTUPINFO startupInfo;
		PROCESS_INFORMATION procInfo;
#else
		int serverPID;
#endif
		
		bool connectedToServer;
		std::shared_ptr<Network> network;
		std::unique_ptr<LevelManager> levelManager;
		CallbackMap callbacks;

		void Disconnect(const Message& m);
		void LevelAction(const Message& m);

	public:

		Game(std::shared_ptr<Engine> engine);
		virtual ~Game();

		virtual bool OnEvent(const irr::SEvent &event);
		virtual void Run();
	};
}

#endif
