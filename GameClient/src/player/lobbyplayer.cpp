#include "player/lobbyplayer.h"

namespace Typhon
{
	LobbyPlayer::LobbyPlayer() 
		: type(AI), name(L"Bot"), perfScore(0), ready(true), refreshTime(0)
	{
		memset(&sourceAddr, 0, sizeof sourceAddr);
		sourceAddr.sin_family = AF_INET;
	}
	
	LobbyPlayer::~LobbyPlayer()
	{
	}
}