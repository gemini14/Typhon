// Project Typhon - Client

#include <exception>
#include <iostream>
#include <sstream>

#include "logger/logger.h"
#include "state/machine.h"

using namespace std;
using namespace Typhon;

// #include <cstdio>

void MessagePump(FSM::Machine &machine);

int main(int argc, char* argv[])
{
	/* This code is only used to determine correct working directory for
	 * exec path creation during game transition.
	FILE *id = popen("echo $PWD", "r");
	char result[100];
	fscanf(id, "%s", result);
	pclose(id);
	cout << result << endl;
	*/
	FSM::Machine machine;
	try
	{
		machine.initiate();
	}
	catch(const std::exception &ex)
	{
		Log(ex.what());
		return 1;
	}

	int lastFPS = -1;

	while(!machine.terminated() && machine.engine->device->run() &&
		!machine.engine->terminate)
	{
		machine.Run(lastFPS);
		MessagePump(machine);
	}

	machine.engine->SavePrefs();

	Logger::Flush();

	return 0;
}

void MessagePump(FSM::Machine &machine)
{
	// process FSM's event queue
	while(!machine.engine->eventQueue.empty())
	{
		auto newEvent = machine.engine->eventQueue.front();
		machine.engine->lang->ClearAllElements();
		switch(newEvent)
		{
		case FSM::GAME:
			/*
			serverThread = new boost::thread(&Server::ServerThreadRun, machine.engine->serverIP);
			*/
			machine.process_event(FSM::EvGame());
			break;

		case FSM::LOBBY:
			machine.process_event(FSM::EvLobby());
			break;

		case FSM::MAINMENU:
			machine.process_event(FSM::EvMainMenu());
			break;

		case FSM::OPTIONS:
			machine.process_event(FSM::EvOptions());
			break;

		case FSM::RET_TO_LOBBY_FROM_GAME:
			/*
			Server::HostLeftGame();
			if(serverThread && serverThread->joinable())
			{
				serverThread->join();
				delete serverThread;
				serverThread = nullptr;
			}
			*/
			machine.process_event(FSM::EvLobby());
			break;

		default:
			Log("Invalid event passed to event queue.");
			break;
		}
		machine.engine->eventQueue.pop();
	}
}
