#ifndef USER_H
#define USER_H

#include <string>

namespace Typhon
{
	class User
	{
	private:

		std::string playerName;
		bool connected;

	public:

		User(const std::string &name);
		User(User&& rhs);
		User& operator=(User&& rhs);
		~User();

		bool GetConnected();
		std::string GetPlayerName();
		void SetConnected(bool c);
	};
}

#endif