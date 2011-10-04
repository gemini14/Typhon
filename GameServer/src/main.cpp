// Project Typhon - Server
// Tarik Sturm-Dahal

#include <unordered_map>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include "logger/logger.h"
#include "network/networkfactory.h"
#include "server/server.h"
#include "server/user.h"
#include "utility/utility.h"

using namespace boost;
using namespace std;
using namespace Typhon;

const int MAX_PLAYERS = 8;

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		// somehow missing one or more setup arguments
		Log("Startup data missing, aborting!");
		return 1;
	}

	boost::char_separator<char> sep("+");
	typedef boost::tokenizer<boost::char_separator<char>> ArgumentTokenizer;
	string arg(argv[1]);
	ArgumentTokenizer tokens(arg, sep);
	
	typedef std::unordered_map<unsigned long, User> PlayerMap;
	PlayerMap players;
	{
		int i = 1;
		unsigned long addr;
		string name;
		for(ArgumentTokenizer::iterator iter = tokens.begin(); iter != tokens.end(); ++iter, ++i)
		{
			Log(*iter);
			if(i % 2)
			{
				name = *iter;
			}
			else
			{
				addr = lexical_cast<unsigned long>(*iter);
				players.insert(PlayerMap::value_type(addr, User(name, HUMAN)));
			}
		}
	}
	int suffix = 1;
	while(players.size() < MAX_PLAYERS)
	{
		string botName("Bot #");
		botName += lexical_cast<string>(suffix++);
		players.insert(PlayerMap::value_type(suffix, User(botName, AI)));
	}
	Log("Player table filled.");

	Log("Starting up server.");
	auto decimalIP = lexical_cast<unsigned long>(argv[2]);
	Server server(GetNetwork(ENETSERVER, PORT_NUMBER, decimalIP), players);
	while(server.Run())
	{
	}
	
	Log("Game is over or host has left, shutting down.");
	Logger::Flush();
	return 0;
}
