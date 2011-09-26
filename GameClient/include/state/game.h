#ifndef GAME_H
#define GAME_H

#include "network/networkfactory.h"
#include "state/fsmstate.h"

namespace Typhon
{
	class Game : public FSMState
	{
	private:

#ifdef WIN32
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
