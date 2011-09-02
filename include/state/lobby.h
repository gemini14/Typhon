#ifndef LOBBY_H
#define LOBBY_H

#include <string>
#include <vector>

#include "network/networkfactory.h"
#include "state/fsmstate.h"

namespace Typhon
{
class Lobby : public FSMState
	{
	private:

		enum PLAYER_TYPE { HUMAN, AI };
		
		class Player
		{
		public:

			Player() : type(AI), name(L"Bot"), perfScore(0), ready(true)
			{
				memset(&sourceAddr, 0, sizeof sourceAddr);
				sourceAddr.sin_family = AF_INET;
			}
			~Player()
			{
			}

			PLAYER_TYPE type;
			std::wstring name;
			int perfScore;
			bool ready;
			sockaddr_in sourceAddr;
		};

		std::unique_ptr<Network> network;
		std::vector<Player> players;
		std::vector<irr::gui::IGUIStaticText*> readyImages;
		int numBots;
		irr::gui::IGUIStaticText *playersGUI;
		std::string discoveryMessage;
		unsigned int prevTime;

		void ChangePlayerReady(const unsigned long playerIP, const char ready = 'S');
		void UpdatePlayersOnScreen();

	public:

		Lobby(std::shared_ptr<Engine> engine);
		virtual ~Lobby();

		void AddPlayer(const std::wstring &name, const int perfScore, const unsigned long location);
		Network* GetNetwork();
		virtual bool OnEvent(const irr::SEvent &event);
		void RemovePlayer(const std::wstring &name);		
		virtual void Run();
	};
}

#endif