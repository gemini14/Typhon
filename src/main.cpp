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
//#include <Windows.h>

#include <iostream>
#include <sstream>

#include "engine/engine.h"
#include "irrlicht/irrlicht.h"
#include "metrics/metrics.h"
#include "network/networkfactory.h"

using namespace irr;
using namespace std;
using namespace Typhon;

const int PORT_NUMBER = 1550;

int main(int argc, char* argv[])
{
	int perfScore = Typhon::Metrics::GetPerfScore();
	cout << "Perf score: " << perfScore << "\n";

	Network *net = GetNetwork(Typhon::RAW, PORT_NUMBER);
	if(!net)
	{
		cout << "Error starting up network code.\n";
		return 1;
	}
	cout << "Network up!\n";

	Engine *engine = new Engine();
	if(!engine->ready)
	{
		cout << "Error creating Irrlicht device.\n";
		delete engine;
		delete net;
		return 1;
	}
	cout << "Irrlicht engine initialized!\n";

	stringstream discovery;
	// discovery << name << "%" << perfScore;
	while(true)
	{
		net->BroadcastMessage(discovery.str(), 'D');
		Message m = net->ReceiveMessage();
		if(m.prefix == 'D')
		{
			std::cout << "Discovery message: " << m.msg;
			break;
		}
	}

	int lastFPS = -1;

	while(engine->device->run())
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
	}

	delete engine;
	delete net;
	return 0;
}
