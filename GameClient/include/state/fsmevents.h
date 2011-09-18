#ifndef FSMEVENTS_H
#define FSMEVENTS_H

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace Typhon
{
	namespace FSM
	{
		enum EVENT_TYPE { GAME, LOBBY, MAINMENU, OPTIONS, RET_TO_LOBBY_FROM_GAME };
		
		struct EvGame : sc::event<EvGame>
		{
		};

		struct EvLobby : sc::event<EvLobby>
		{
		};

		struct EvMainMenu : sc::event<EvMainMenu>
		{
		};

		struct EvOptions : sc::event<EvOptions>
		{
		};
	}
}

#endif