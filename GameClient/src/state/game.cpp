#include "state/game.h"

#include "utility/stateexception.h"

using namespace irr;
using namespace std;

namespace Typhon
{
	enum GUI_IDS {
	};

	Game::Game(std::shared_ptr<Engine> engine)
		: FSMState(engine), network(Typhon::GetNetwork(Typhon::ENETCLIENT, PORT_NUMBER, &engine->serverIP))
	{
		if (!network)
		{
			throw StateException("Error starting up network code (could not allocate or incompatible system).\n");
		}
	}

	Game::~Game()
	{
	}

	bool Game::OnEvent(const irr::SEvent &event)
	{
		switch(event.EventType)
		{
		case EET_GUI_EVENT:
			{
				s32 id = event.GUIEvent.Caller->getID();
			}
			break;

		case EET_KEY_INPUT_EVENT:
			{
				auto key = event.KeyInput.Key;

				switch(key)
				{
				case KEY_ESCAPE:
					engine->eventQueue.push(FSM::RET_TO_LOBBY_FROM_GAME);
					break;
				}
			}
			break;

		default:
			break;
		}

		return false;
	}

	void Game::Run()
	{
	}
}