#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <memory>
#include <string>

#include <boost/mpl/list.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>

#include "engine/engine.h"
#include "metrics/metrics.h"
#include "network/networkfactory.h"
#include "state/mainmenu.h"
#include "utility/stateexception.h"

namespace mpl = boost::mpl;
namespace sc = boost::statechart;


namespace Typhon
{
	namespace FSM
	{
		// Forward Declarations
		struct Game;
		struct Lobby;
		struct MainMenu;
		struct Options;

		// EVENTS
		struct EvGame: sc::event<EvGame>
		{
		};
		struct EvLobby: sc::event<EvLobby>
		{
		};
		struct EvMainMenu: sc::event<EvMainMenu>
		{
		};
		struct EvOptions: sc::event<EvOptions>
		{
		};

		// MACHINE
		struct Machine: sc::state_machine<Machine, MainMenu>
		{
			typedef void (*StateRun)();

			StateRun stateRun;
			std::shared_ptr<Engine> engine;
			const int perfScore;

			Machine() :
			engine(new Engine()), perfScore(Typhon::Metrics::GetPerfScore())
			{
				if (!engine || !engine->ready)
				{
					throw StateException("Couldn't allocate or create Irrlicht device.\n");
				}

				std::cout << "Irrlicht engine initialized!\n";
				std::cout << "Perf score: " << perfScore << "\n";
			}

			void Run(int &lastFPS)
			{
				engine->driver->beginScene();

				// state-specific code
				// to change this, the individual states assign the machine's stateRun function pointer
				// to their version of stateRun using outermost_context
				stateRun();

				engine->smgr->drawAll();
				engine->gui->drawAll();
				engine->driver->endScene();

				int fps = engine->driver->getFPS();
				if (lastFPS != fps)
				{
					irr::core::stringw titleBar(L"Project Typhon - FPS: ");
					titleBar += fps;
					engine->device->setWindowCaption(titleBar.c_str());
					lastFPS = fps;
				}
			}
		};

		// STATES
		struct Game: sc::state<Game, Machine>
		{
			typedef mpl::list<sc::transition<EvLobby, Lobby>> reactions;

			static void StateRun()
			{
			}

			Game(my_context ctx) :
			my_base(ctx)
			{
				std::cout << "Entered the game! Hi hi!\n";
				outermost_context().stateRun = &StateRun;
			}

			~Game()
			{
				std::cout << "Game bye!\n";
			}
		};

		struct Lobby: sc::state<Lobby, Machine>
		{
			typedef mpl::list<sc::transition<EvMainMenu, MainMenu>, sc::transition<
				EvGame, Game>> reactions;

			std::unique_ptr<Network> network;

			static void StateRun()
			{
				std::cout << "OoooOOoooOo we're in the lobby!\n";
			}

			Lobby(my_context ctx) :
			my_base(ctx), network(Typhon::GetNetwork(Typhon::RAW, PORT_NUMBER))
			{
				if (!network)
				{
					throw StateException("Error starting up network code (could not allocate or incompatible system).\n");
				}
				std::cout << "Network up!\n";
				std::cout << "Lobby hi!\n";
				outermost_context().stateRun = &StateRun;
			}

			~Lobby()
			{
				std::cout << "Lobby bye!\n";
			}
		};

		struct MainMenu: sc::state<MainMenu, Machine>
		{
			typedef mpl::list<sc::transition<EvOptions, Options>, sc::transition<
				EvLobby, Lobby>> reactions;

			std::unique_ptr<Typhon::MainMenu> menu;

			static void StateRun()
			{
				std::cout << "Wow it works! On Menu!\n";
			}

			MainMenu(my_context ctx) :
			my_base(ctx)
			{
				std::cout << "Hi hi!\n";
				outermost_context().stateRun = &StateRun;
			}

			~MainMenu()
			{
				std::cout << "Bye bye!\n";
			}
		};

		struct Options: sc::state<Options, Machine>
		{
			typedef sc::transition<EvMainMenu, MainMenu> reactions;

			static void StateRun()
			{
				std::cout << "No way..we're changing options!\n";
			}

			Options(my_context ctx) :
			my_base(ctx)
			{
				std::cout << "Options hi!\n";
				outermost_context().stateRun = &StateRun;
			}

			~Options()
			{
				std::cout << "Options bye!\n";
			}
		};
	}
}

#endif