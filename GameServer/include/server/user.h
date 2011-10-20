#ifndef USER_H
#define USER_H

#include <string>

namespace Typhon
{
	enum PLAYER_TYPE { HUMAN, AI };

	class User
	{
	private:

		std::string playerName;
		bool connected;
		PLAYER_TYPE type;

	public:

		User(const std::string &name, const PLAYER_TYPE playerType);
		User(const User& rhs);
		User(User&& rhs);
		User& operator=(User&& rhs);
		~User();

		bool GetConnected();
		std::string GetPlayerName();
		PLAYER_TYPE GetType();
		void SetConnected(bool c);
	};
}

#endif
