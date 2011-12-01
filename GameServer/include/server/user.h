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
		unsigned int playerID;
		bool connected;
		PLAYER_TYPE type;

	public:

		User(const std::string &name, const unsigned int ID, const PLAYER_TYPE playerType);
		User(const User& rhs);
		User(User&& rhs);
		User& operator=(User&& rhs);
		~User();

		bool GetConnected();
		unsigned int GetID();
		std::string GetPlayerName();
		PLAYER_TYPE GetType();
		void SetConnected(bool c);
	};
}

#endif
