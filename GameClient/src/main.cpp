/*
Copyright (c) 2011 Tarik Sturm-Dahal

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, 
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
*/

// Project Typhon

#include <exception>
#include <iostream>
#include <sstream>

#include <boost/thread.hpp>

#include "logger/logger.h"
#include "server/server.h"
#include "state/machine.h"

using namespace boost;
using namespace std;
using namespace Typhon;

void MessagePump(FSM::Machine &machine, boost::thread* &serverThread);

int main(int argc, char* argv[])
{
	boost::thread loggerThread(&Logger::Run);

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
	boost::thread *serverThread = nullptr;

	while(!machine.terminated() && machine.engine->device->run() &&
		!machine.engine->terminate)
	{
		machine.Run(lastFPS);
		MessagePump(machine, serverThread);		
	}

	machine.engine->SavePrefs();

	Logger::FlushAndExit();
	if(loggerThread.joinable())
	{
		loggerThread.join();
	}

	return 0;
}

void MessagePump(FSM::Machine &machine, boost::thread* &serverThread)
{
	// process FSM's event queue
	while(!machine.engine->eventQueue.empty())
	{
		auto newEvent = machine.engine->eventQueue.front();
		machine.engine->lang->ClearAllElements();
		switch(newEvent)
		{
		case FSM::GAME:
			serverThread = new boost::thread(&Server::ServerThreadRun, machine.engine->serverIP);
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
			Server::HostLeftGame();
			if(serverThread && serverThread->joinable())
			{
				serverThread->join();
				delete serverThread;
				serverThread = nullptr;
			}
			machine.process_event(FSM::EvLobby());
			break;

		default:
			Log("Invalid event passed to event queue.");
			break;
		}
		machine.engine->eventQueue.pop();
	}
}