#ifndef GAME_H
#define GAME_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "network/networkfactory.h"
#include "state/fsmstate.h"

namespace Typhon
{
	class Game : public FSMState
	{
	private:

#ifdef WIN32
		STARTUPINFO startupInfo;
		PROCESS_INFORMATION procInfo;
#else
		int serverPID;
#endif
		std::unique_ptr<Network> network;

	public:

		Game(std::shared_ptr<Engine> engine);
		virtual ~Game();

		virtual bool OnEvent(const irr::SEvent &event);
		virtual void Run();
	};
}

#endif
