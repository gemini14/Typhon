#ifndef LOBBY_H
#define LOBBY_H

#include <string>
#include <vector>

#include "network/networkfactory.h"
#include "state/fsmstate.h"

namespace Typhon
{
	class Lobby : public FSMState
	{
	private:

		struct Player
		{
			enum PLAYER_TYPE { AI, HUMAN };

			PLAYER_TYPE type;
			std::wstring name;
			int perfScore;
			sockaddr *sourceAddr;
		};

		std::unique_ptr<Network> network;
		std::vector<Player> playerList;

	public:

		Lobby(std::shared_ptr<Engine> engine);
		virtual ~Lobby();

		virtual bool OnEvent(const irr::SEvent &event);
		virtual void Run();
	};
}

#endif