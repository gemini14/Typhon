#ifndef LOBBY_H
#define LOBBY_H

#include <string>
#include <vector>

#include "network/networkfactory.h"
#include "player/lobbyplayer.h"
#include "state/fsmstate.h"

namespace Typhon
{
class Lobby : public FSMState
	{
	private:

		std::unique_ptr<Network> network;
		std::vector<LobbyPlayer> players;
		std::vector<irr::gui::IGUIStaticText*> readyImages;
		int numBots;
		irr::gui::IGUIStaticText *playersGUI;
		std::string discoveryMessage;
		unsigned int prevTime;
		LobbyPlayer designatedServer;

		void BroadcastPlayerInfoStatus();
		void ChangePlayerReady(const unsigned long playerIP, const char ready = 'S');
		void CheckNewServerCandidate(const LobbyPlayer& newPlayer);
		void PruneDisconnects();
		void UpdatePlayersOnScreen();
		void UpdateReadyBoxes();

	public:

		Lobby(std::shared_ptr<Engine> engine);
		virtual ~Lobby();

		void AddPlayer(const std::wstring &name, const int perfScore, const unsigned long location);
		Network* GetNetwork();
		virtual bool OnEvent(const irr::SEvent &event);
		bool ReadyToPlay();
		void RemovePlayer(const unsigned long addr);		
		virtual void Run();
	};
}

#endif