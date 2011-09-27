// Project Typhon - Server
// Tarik Sturm-Dahal

#include <cstdlib>
#include <iostream>

#include "logger/logger.h"
#include "network/networkfactory.h"

using namespace Typhon;

int main(int argc, char *argv[])
{
	Log("No way, it actually worked!!");
	Log(argv[1]);
	Log("This will exit immediately after this statement...server out!");
	std::cin.get();
	return 0;
}
