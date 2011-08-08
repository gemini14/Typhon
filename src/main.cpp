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

#include <iostream>
#include <sstream>

#include "state/machine.h"

using namespace irr;
using namespace std;
using namespace Typhon;


int main(int argc, char* argv[])
{
	Machine machine;
	try
	{
		machine.initiate();
	}
	catch(const std::exception &ex)
	{
		std::cout << ex.what();
		return 1;
	}
	machine.process_event(EvOptions());
	machine.process_event(EvMainMenu());
	machine.process_event(EvLobby());
	machine.process_event(EvMainMenu());

	stringstream discovery;
	// discovery << name << "%" << perfScore;
	/*while(true)
	{
	net->BroadcastMessage(discovery.str(), 'D');
	Message m = net->ReceiveMessage();
	if(m.prefix == 'D')
	{
	std::cout << "Discovery message: " << m.msg;
	break;
	}
	}*/

	int lastFPS = -1;

	/*while(engine->device->run())
	{
		engine->driver->beginScene();
		engine->smgr->drawAll();
		engine->gui->drawAll();
		engine->driver->endScene();

		int fps = engine->driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw titleBar(L"Project Typhon - FPS: ");
			titleBar += fps;
			engine->device->setWindowCaption(titleBar.c_str());
			lastFPS = fps;
		}
	}*/

	//delete engine;
	//delete net;
	return 0;
}
