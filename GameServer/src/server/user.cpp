#include "server/user.h"


#include <utility>


namespace Typhon
{
	User::User(const std::string &name, const PLAYER_TYPE playerType)
		: playerName(name), connected(false), type(playerType)
	{
	}

	User::User(const User& rhs)
	{
		playerName = rhs.playerName;
		type = rhs.type;
		connected = rhs.connected;
	}

	User::User(User&& rhs)
	{
		*this = std::move(rhs);
	}

	User& User::operator=(User&& rhs)
	{
		if(this != &rhs)
		{
			playerName = rhs.playerName;
			type = rhs.type;
			connected = rhs.connected;
		}
		return *this;
	}

	User::~User()
	{
	}

	bool User::GetConnected()
	{
		return connected;
	}

	std::string User::GetPlayerName()
	{
		return playerName;
	}
		
	PLAYER_TYPE User::GetType()
	{
		return type;
	}

	void User::SetConnected(bool c)
	{
		connected = c;
	}
}
