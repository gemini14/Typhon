#include "server/user.h"


#include <utility>


namespace Typhon
{
	User::User(const std::string &name)
		: playerName(name), connected(false)
	{
	}

	User::User(User&& rhs)
		: playerName("")
	{
		*this = std::move(rhs);
	}

	User& User::operator=(User&& rhs)
	{
		if(this != &rhs)
		{
			playerName = rhs.playerName;
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

	void User::SetConnected(bool c)
	{
		connected = c;
	}
}