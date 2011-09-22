#ifndef LOBBYPLAYER_H
#define LOBBYPLAYER_H

#include <string>

#ifdef WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

namespace Typhon
{
	enum PLAYER_TYPE { HUMAN, AI };

	class LobbyPlayer
	{
	public:

		LobbyPlayer();
		~LobbyPlayer();
		// compiler generated copy/assign OK

		PLAYER_TYPE type;
		std::wstring name;
		int perfScore;
		bool ready;
		sockaddr_in sourceAddr;
		int refreshTime;
	};
}

#endif