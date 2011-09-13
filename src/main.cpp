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

#include "state/machine.h"

using namespace std;
using namespace Typhon;


int main(int argc, char* argv[])
{
	FSM::Machine machine;
	try
	{
		machine.initiate();
	}
	catch(const exception &ex)
	{
		cout << ex.what();
		return 1;
	}

	int lastFPS = -1;
	boost::thread *serverThread = nullptr;

	while(!machine.terminated() && machine.engine->device->run() &&
			!machine.engine->terminate)
	{
		machine.Run(lastFPS);
		
		// process FSM's event queue
		while(!machine.engine->eventQueue.empty())
		{
			auto newEvent = machine.engine->eventQueue.front();
			machine.engine->lang->ClearAllElements();
			switch(newEvent)
			{
			case FSM::GAME:
				// serverThread = new boost::thread(
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

			default:
				cout << "Invalid event passed to event queue.\n";
				break;
			}
			machine.engine->eventQueue.pop();
		}
	}

	machine.engine->SavePrefs();
	//stringstream discovery;
	//discovery << name << "%" << perfScore;
	//while(true)
	//{
	//	net->BroadcastMessage(discovery.str(), 'D');
	//	Message m = net->ReceiveMessage();
	//	if(m.prefix == 'D')
	//	{
	//		std::cout << "Discovery message: " << m.msg;
	//		break;
	//	}
	//}

	return 0;
}
