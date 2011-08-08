#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <memory>

#include <boost/mpl/list.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>

#include "engine/engine.h"
#include "metrics/metrics.h"
#include "network/networkfactory.h"

namespace mpl = boost::mpl;
namespace sc = boost::statechart;

using namespace sc;

namespace Typhon
{
	// Forward Declarations
	struct Lobby;
	struct MainMenu;
	struct Options;


	// EVENTS
	struct EvGame : sc::event<EvGame>{};
	struct EvLobby : sc::event<EvLobby>{};
	struct EvMainMenu : sc::event<EvMainMenu>{};
	struct EvOptions : sc::event<EvOptions>{};	


	// MACHINE
	struct Machine : sc::state_machine<Machine, MainMenu>
	{
		std::unique_ptr<Engine> engine;
		int perfScore;

		Machine() : engine(new Engine()), perfScore(Typhon::Metrics::GetPerfScore())
		{
			if(!engine || !engine->ready)
			{
				throw std::exception("Couldn't allocate or create Irrlicht device.\n");
			}

			std::cout << "Irrlicht engine initialized!\n";
			std::cout << "Perf score: " << perfScore << "\n";
		}
	};


	// STATES
	struct Lobby : sc::simple_state<Lobby, Machine>
	{
		typedef mpl::list<
			sc::transition<EvMainMenu, MainMenu>> reactions;
		
		std::unique_ptr<Network> network;

		Lobby() : network(Typhon::GetNetwork(Typhon::RAW, PORT_NUMBER))
		{
			if(!network)
			{
				throw std::exception("Error starting up network code (could not allocate or incompatible system).\n");
			}
			std::cout << "Network up!\n";
			std::cout << "Lobby hi!\n";
		}

		~Lobby()
		{
			std::cout << "Lobby bye!\n";
		}
	};

	struct MainMenu : sc::simple_state<MainMenu, Machine>
	{
		typedef mpl::list<
			sc::transition<EvOptions, Options>,
			sc::transition<EvLobby, Lobby>> reactions;

		MainMenu()
		{
			std::cout << "Hi hi!\n";
		}

		~MainMenu()
		{
			std::cout << "Bye bye!\n";
		}
	};

	struct Options : sc::simple_state<Options, Machine>
	{
		typedef sc::transition<EvMainMenu, MainMenu> reactions;

		Options()
		{
			std::cout << "Options hi!\n";
		}

		~Options()
		{
			std::cout << "Options bye!\n";
		}
	};
}

#endif