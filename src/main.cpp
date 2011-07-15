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

#include <irrlicht/irrlicht.h>

#include <metrics/metrics.h>
#include <network/network.h>

int main(int argc, char* argv[])
{
	// irr::c8 test;
	std::cout << "Perf score: " << Typhon::Metrics::GetPerfScore() << "\n";
	Typhon::Network net(Typhon::RAW, 1550);
	bool ok = net.StartUp();
	char buf[INET_ADDRSTRLEN];
	std::cout << "Listening...\n";
	while(true)
	{
		//net.BroadcastMessage("Herro, I'm Charlie the Chimp!", 'D');
		Typhon::Message m = net.ReceiveMessage();
		if(m.prefix == 'D')
		{
			std::cout << "Hey, we got a message!  Prefix: " << m.prefix << " IP: "
				<< inet_ntop(AF_INET, &m.address, buf, INET_ADDRSTRLEN) << "\nMessage reads: " << m.msg;
			break;
		}
		else
		{
			std::cout << "Nothing yet...\n";
		}
		//Sleep(2000);
	}
	return 0;
}
