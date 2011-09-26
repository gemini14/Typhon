#include "state/game.h"

#ifdef WIN32
#else
#include <cstdio>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

#include <boost/lexical_cast.hpp>

#include "utility/stateexception.h"

using namespace irr;
using namespace std;

namespace Typhon
{
	enum GUI_IDS
	{
	};

	Game::Game(std::shared_ptr<Engine> engine) :
			FSMState(engine), serverPID(0), network(
					Typhon::GetNetwork(Typhon::ENETCLIENT, PORT_NUMBER,
							&engine->serverIP))
	{
		if (!network)
		{
			throw StateException(
					"Error starting up network code (could not allocate or incompatible system).\n");
		}

		if (engine->clientIsServer)
		{
			// create child process to run the server (if this client is the
			// one chosen to run the server)
#ifdef WIN32
#else
			serverPID = fork();
			if (serverPID == -1)
			{
				throw StateException("Error forking client.\n");
			}
			else if (serverPID == 0)
			{
				// this is the child
				// run the server program, sending the names & IPs
				// of the players as an argument
				std::unique_ptr<char[]> lobbyList(new char[engine->lobbyList->length() + 1]);
				strcpy(lobbyList.get(), engine->lobbyList->c_str());
				char * const argList[] = { "TyphonServer", lobbyList.get(), nullptr };

				// TODO Important -- these paths MUST be adjusted for final release
#ifdef DEBUG
				execvp("../../bin/Server/Linux/TyphonServer_D", argList);
#else
				execvp("../../bin/Server/Linux/TyphonServer", argList);
#endif
				// if we reach this point, something went wrong (exec doesn't return
				// otherwise)
				perror("execvp");
				Log("Exec call to create server failed.");
				throw StateException("Error exec'ing server.\n");
			}

			// if we reached this point, we're the parent
#endif
		}
	}

	Game::~Game()
	{
		if (engine->clientIsServer)
		{
			network->BroadcastMessage("", 'Q');
			int status;
			waitpid(serverPID, &status, 0);
			engine->clientIsServer = false;
			engine->lobbyList.reset();
		}
	}

	bool Game::OnEvent(const irr::SEvent &event)
	{
		switch (event.EventType)
		{
			case EET_GUI_EVENT:
			{
				s32 id = event.GUIEvent.Caller->getID();
			}
				break;

			case EET_KEY_INPUT_EVENT:
			{
				auto key = event.KeyInput.Key;

				switch (key)
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
