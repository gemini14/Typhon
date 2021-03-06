#include "state/game.h"

#ifndef WIN32
#include <cstdio>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

#include <boost/lexical_cast.hpp>

#include "network/networkenetclient.h"
#include "utility/stateexception.h"
#include "utility/utility.h"

using namespace irr;
using namespace std;

namespace Typhon
{
	void Game::Disconnect(const Message& m)
	{
		if(connectedToServer)
		{
			connectedToServer = false;
			Log("Server disconnected.");
			// TODO: Add some kind of notification or log system so user can see what happened
			engine->eventQueue.push(FSM::RET_TO_LOBBY_FROM_GAME);
		}		
	}

	void Game::LevelAction(const Message& m)
	{
		levelManager->PushMessage(m);
	}

	Game::Game(std::shared_ptr<Engine> engine) :
	FSMState(engine), 
#ifndef WIN32
		serverPID(0),
#endif
		connectedToServer(false), network(nullptr), levelManager(new LevelManager(engine))
	{
		callbacks['D'] = &Game::Disconnect;
		callbacks['L'] = &Game::LevelAction;

		if (engine->clientIsServer)
		{
			// create child process to run the server (if this client is the
			// one chosen to run the server)
#ifdef WIN32

			memset(&startupInfo, 0, sizeof startupInfo);
			memset(&procInfo, 0, sizeof procInfo);
			startupInfo.cb = sizeof startupInfo;
			std::string temp("TyphonServer ");
			temp += *engine->lobbyList;
			temp += " " + boost::lexical_cast<string>(GetNetworkIP(engine->serverIP));
			LPSTR argLine = const_cast<char*>(temp.c_str());

			int result;
#ifdef WIN64
#ifdef DEBUG // 64-bit build, Debug
			result = CreateProcess("../../bin/Server/Win64/Debug/TyphonServer64.exe", argLine, 
				nullptr, nullptr, true, CREATE_NEW_CONSOLE, nullptr, nullptr,
				&startupInfo, &procInfo);
#else // 64-bit build, Release
			result = CreateProcess("../../bin/Server/Win64/Release/TyphonServer64.exe", argLine, 
				nullptr, nullptr, true, CREATE_NEW_CONSOLE, nullptr, nullptr,
				&startupInfo, &procInfo);
#endif
#elif defined(WIN32)
#ifdef DEBUG // 32-bit build, Debug
			result = CreateProcess("../../bin/Server/Win32/Debug/TyphonServer32.exe", argLine, 
				nullptr, nullptr, true, CREATE_NEW_CONSOLE, nullptr, nullptr,
				&startupInfo, &procInfo);
#else // 32-bit build, Release
			result = CreateProcess("../../bin/Server/Win32/Release/TyphonServer32.exe", argLine, 
				nullptr, nullptr, true, CREATE_NEW_CONSOLE, nullptr, nullptr,
				&startupInfo, &procInfo);
#endif
#endif
			if(!result)
			{
				throw StateException("Error creating server process.\n");
			}
			//else
			//{
			//	auto waitResult = WaitForInputIdle(procInfo.hProcess, INFINITE);
			//}
#endif
#ifndef WIN32 // Linux
			std::unique_ptr<char[]> lobbyList(new char[engine->lobbyList->length() + 1]);
			strcpy(lobbyList.get(), engine->lobbyList->c_str());
			string server(boost::lexical_cast<string>(GetNetworkIP(engine->serverIP)));
			std::unique_ptr<char[]> serverAddr(new char[server.length() + 1]);
			strcpy(serverAddr.get(), server.c_str());
			char * const argList[] = { "TyphonServer", *lobbyList, *serverAddr, nullptr };

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

		network.reset(Typhon::GetNetwork(Typhon::ENETCLIENT, PORT_NUMBER, &engine->serverIP));
		if (!network)
		{
			// TODO: Add some kind of notification if user can't initially connect
			throw StateException(
				"Error starting up network code (could not allocate or incompatible system).\n");
		}
		// set the level manager's network
		levelManager->network = network;
		
		connectedToServer = reinterpret_cast<NetworkENetClient*>(network.get())->ConnectToServer();
		if(!connectedToServer)
		{
			engine->eventQueue.push(FSM::RET_TO_LOBBY_FROM_GAME);
		}
	}

	Game::~Game()
	{
		auto rootNode = engine->smgr->getRootSceneNode();
		if(rootNode)
		{
			rootNode->removeAll();
			rootNode->remove();
		}

		if (engine->clientIsServer)
		{
			reinterpret_cast<NetworkENetClient*>(network.get())->DisconnectFromServer();
#ifdef WIN32
			CloseHandle(startupInfo.hStdError);
			CloseHandle(startupInfo.hStdInput);
			CloseHandle(startupInfo.hStdOutput);
			CloseHandle(procInfo.hThread);
			CloseHandle(procInfo.hProcess);
#else
			int status;
			waitpid(serverPID, &status, 0);
#endif
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
				/*s32 id = event.GUIEvent.Caller->getID();
				switch(event.GUIEvent.EventType)
				{
				
				default:
					break;
				}*/
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
		if(!connectedToServer)
		{
			return;
		}

		static auto previousTime = engine->device->getTimer()->getTime();
		auto currentTime = engine->device->getTimer()->getTime();
		if(currentTime - previousTime >= 5000)
		{
			// refresh connection every 5 sec
			network->BroadcastMessage("", 'A');
			previousTime = currentTime;
		}

		auto message = network->ReceiveMessage();
		if(message.prefix != 'N')
		{
			// need to dereference the stored member function pointer and call
			// it using "this", all wrapped up in parentheses
			(this->*callbacks[message.prefix])(message);
		}
		
		levelManager->Run();
	}
}
