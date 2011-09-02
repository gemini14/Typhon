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
#include "state/fsmevents.h"
#include "state/game.h"
#include "state/lobby.h"
#include "state/mainmenu.h"
#include "state/options.h"
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

		// MACHINE
		struct Machine : sc::state_machine<Machine, MainMenu>
		{
			std::shared_ptr<FSMState> state;
			std::shared_ptr<Engine> engine;
			const int perfScore;
			irr::video::SColor backColor;

			Machine()
				: engine(new Engine()), perfScore(Typhon::Metrics::GetPerfScore()), backColor(255, 255, 255, 255)
			{
				if (!engine || !engine->ready)
				{
					throw StateException("Couldn't allocate or create Irrlicht device.\n");
				}

				std::cout << "Irrlicht engine initialized.\n";
				std::cout << "Perf score: " << perfScore << "\n";
			}

			void Run(int &lastFPS)
			{
				engine->driver->beginScene(true, true, backColor); 

				state->Run();

				engine->smgr->drawAll();
				engine->gui->drawAll();
				engine->driver->endScene();

				int fps = engine->driver->getFPS();
				if (lastFPS != fps)
				{
					irr::core::stringw titleBar(L"Lunatic Lanes - FPS: ");
					titleBar += fps;
					engine->device->setWindowCaption(titleBar.c_str());
					lastFPS = fps;
				}
			}
		};

		// STATES
		struct Game : sc::state<Game, Machine>
		{
			typedef mpl::list<
				sc::transition<EvLobby, Lobby>> reactions;

			Game(my_context ctx)
				: my_base(ctx)
			{
				std::cout << "Entered the game state\n";
				outermost_context().state.reset();
			}

			~Game()
			{
				std::cout << "Game state exit\n";
			}
		};

		struct Lobby : sc::state<Lobby, Machine>
		{
			typedef mpl::list<
				sc::transition<EvMainMenu, MainMenu>,
				sc::transition<EvGame, Game>> reactions;

			std::shared_ptr<Typhon::Lobby> lobby;

			Lobby(my_context ctx)
				: my_base(ctx), lobby(new Typhon::Lobby(outermost_context().engine))
			{
				lobby->AddPlayer(lobby->engine->options.name, outermost_context().perfScore,
					lobby->GetNetwork()->GetIP(), lobby->GetNetwork()->portNumber);
				outermost_context().state.reset();
				outermost_context().state = lobby;
				outermost_context().backColor = irr::video::SColor(255, 245, 203, 10);
				std::cout << "Network up!\n";
				std::cout << "Lobby hi!\n";
			}

			~Lobby()
			{
				std::cout << "Lobby bye!\n";
			}
		};

		struct MainMenu : sc::state<MainMenu, Machine>
		{
			typedef mpl::list<
				sc::transition<EvOptions, Options>,
				sc::transition<EvLobby, Lobby>> reactions;

			std::shared_ptr<Typhon::MainMenu> menu;

			MainMenu(my_context ctx)
				: my_base(ctx), menu(new Typhon::MainMenu(outermost_context().engine))
			{
				outermost_context().state.reset();
				outermost_context().state = menu;
				outermost_context().backColor = irr::video::SColor(255, 245, 203, 10);
			}

			~MainMenu()
			{
			}
		};

		struct Options : sc::state<Options, Machine>
		{
			typedef sc::transition<EvMainMenu, MainMenu> reactions;

			std::shared_ptr<Typhon::Options> options;

			Options(my_context ctx)
				: my_base(ctx), options(new Typhon::Options(outermost_context().engine))
			{
				outermost_context().state.reset();
				outermost_context().state = options;
				outermost_context().backColor = irr::video::SColor(255, 245, 203, 10);
			}

			~Options()
			{
			}
		};
	}
}

#endif