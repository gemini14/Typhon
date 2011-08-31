#ifndef LOBBY_H
#define LOBBY_H

#include "network/networkfactory.h"
#include "state/fsmstate.h"

namespace Typhon
{
	class Lobby : public FSMState
	{
	private:

		std::unique_ptr<Network> network;

	public:

		Lobby(std::shared_ptr<Engine> engine);
		virtual ~Lobby();

		virtual bool OnEvent(const irr::SEvent &event);
		virtual void Run();
	};
}

#endif