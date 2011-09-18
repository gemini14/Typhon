#ifndef LOBBYLIST_H
#define LOBBYLIST_H

#include <boost/noncopyable.hpp>
#include <functional>
#include <map>
#include <string>

namespace Typhon
{
	class LobbyList : boost::noncopyable
	{
	private:

		struct PlayerInfo
		{
			std::string name;
			unsigned long ipAddr;
			bool readyToStart;
		};

		std::multimap<int, PlayerInfo, std::greater<int>> players;

	public:

		LobbyList();
		~LobbyList();

		void AddPlayer(const int perfScore, const std::string &name, const unsigned long ipAddr);
		bool AllReady();
		unsigned long GetServerIP(); // should I use ulong or struct?
		void RemovePlayer(const unsigned long ipAddr);
	};
}

#endif