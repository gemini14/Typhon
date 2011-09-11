#include "state/lobby.h"

#include <algorithm>
#ifndef DEBUG
#define NDEBUG
#endif
#include <cassert>
#include <iostream>
#include <list>
#include <regex>
#include <sstream>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include "engine/engine.h"
#include "utility/stateexception.h"
#include "utility/utility.h"

using namespace irr;
using namespace std;

namespace Typhon
{
	const video::SColor RED = video::SColor(255, 255, 0, 0);
	const video::SColor GREEN = video::SColor(255, 0, 255, 0);

	const int BROADCAST_INTERVAL = 1000;
	// if we haven't heard another discovery message from player in PLAYER_TIMEOUT seconds, 
	// assume they quit the program or program was force closed, cable ripped out, etc.
	const int PLAYER_TIMEOUT = 3000;

	// GUI ENUMS
	enum GUI_ID { 
		TABCONTROL_PLAYERPANE,

		BUTTON_READY,
		BUTTON_RETURN_MENU,

		STATIC_TEXT_PLAYER,
		STATIC_TEXT_PLAYER_LIST,
		STATIC_TEXT_READY
	};

	void Lobby::BroadcastPlayerInfoStatus()
	{
		network->BroadcastMessage(discoveryMessage, 'D');
		auto iter = find_if(players.begin(), players.end(), [=](const LobbyPlayer &p)
		{
			return GetNetworkIP(p.sourceAddr) == network->GetIP();
		});
		if(iter != players.end())
		{
			switch(iter->ready)
			{
			case true:
				network->BroadcastMessage("T", 'R');
				break;
			case false:
				network->BroadcastMessage("F", 'R');
				break;
			}
		}
	}

	void Lobby::ChangePlayerReady(const unsigned long playerIP, const char ready)
	{
		for(int i = 0; i < MAX_PLAYERS; ++i)
		{
			if(GetNetworkIP(players[i].sourceAddr) == playerIP)
			{
				if(ready == 'S')
				{
					players[i].ready = !players[i].ready;
				}
				else if(ready == 'T')
				{
					players[i].ready = true;
				}
				else
				{
					players[i].ready = false;
				}

				break;
			}
		}

		UpdateReadyBoxes();
	}

	void Lobby::CheckNewServerCandidate(const LobbyPlayer& newPlayer)
	{
		if(newPlayer.type == AI)
		{
			return;
		}

		if(designatedServer.perfScore == 0)
		{
			designatedServer = newPlayer;
		}
		else if(designatedServer.perfScore == newPlayer.perfScore &&
			GetNetworkIP(designatedServer.sourceAddr) < GetNetworkIP(newPlayer.sourceAddr))
		{
			designatedServer = newPlayer;
		}
	}

	void Lobby::PruneDisconnects()
	{
		list<unsigned long> playersToRemove;
		BOOST_FOREACH(const LobbyPlayer &p, players)
		{
			// following if needs this->HUMAN to compile, not sure why, since the
			// lambda should capture it ([&])
			if(p.type == HUMAN && p.refreshTime >= PLAYER_TIMEOUT && GetNetworkIP(p.sourceAddr) != network->GetIP())
			{
				playersToRemove.push_back(GetNetworkIP(p.sourceAddr));
				wcout << p.name;
				cout << " pruned, refresh is " << p.refreshTime << "\n\n";
			}
		}

		BOOST_FOREACH(const unsigned long addr, playersToRemove)
		{
			RemovePlayer(addr);
		}
	}

	bool Lobby::ReadyToPlay()
	{
		// there must be at least 1 human player in the lobby
		if(numBots == MAX_PLAYERS)
		{
			return false;
		}

		bool ready = true;
		BOOST_FOREACH(const LobbyPlayer p, players)
		{
			if(!p.ready)
			{
				ready = false;
			}
		}

		return ready;
	}

	void Lobby::UpdatePlayersOnScreen()
	{
		std::wstringstream playerText;
		// tried to use std::copy and ostream_iterator, but it was such a pain
		// to (try to) get it working that I just went with a simple for loop to save time
		BOOST_FOREACH(const LobbyPlayer iter, players)
		{
			playerText << iter.name << L"\n\n";
		}
		playersGUI->setText(playerText.str().c_str());
		UpdateReadyBoxes();
	}

	void Lobby::UpdateReadyBoxes()
	{
		for(int i = 0; i < MAX_PLAYERS; ++i)
		{
			if(players[i].ready)
			{
				readyImages[i]->setBackgroundColor(GREEN);
			}
			else
			{
				readyImages[i]->setBackgroundColor(RED);
			}
		}
	}

	Lobby::Lobby(std::shared_ptr<Engine> engine)
		: FSMState(engine), network(Typhon::GetNetwork(Typhon::RAW, PORT_NUMBER)),
		players(MAX_PLAYERS, LobbyPlayer()), numBots(MAX_PLAYERS),
		discoveryMessage(ConvertWideToStr(engine->options.name) + " " + boost::lexical_cast<std::string>(engine->perfScore))
	{
		if (!network)
		{
			throw StateException("Error starting up network code (could not allocate or incompatible system).\n");
		}

		prevTime = engine->device->getTimer()->getTime();
		// player pane (holds list of human players/AI who will be playing them game)
		guiElements.push_back(engine->gui->addTabControl(core::rect<s32>(
			GUI_ELEMENT_SPACING * 2,
			GUI_ELEMENT_SPACING * 2,
			edgeBorderWidth,
			edgeBorderHeight - BUTTON_HEIGHT - GUI_ELEMENT_SPACING),
			0,
			false,
			true,
			TABCONTROL_PLAYERPANE));

		// Return to main menu button
		guiElements.push_back(engine->gui->addButton(core::rect<s32>(
			edgeBorderWidth - BUTTON_WIDTH * 2 - GUI_ELEMENT_SPACING,
			edgeBorderHeight - BUTTON_HEIGHT,
			edgeBorderWidth - BUTTON_WIDTH - GUI_ELEMENT_SPACING,
			edgeBorderHeight), 
			0, 
			BUTTON_RETURN_MENU)); 
		engine->lang->AddElementWithText(guiElements.back(), "BackToMainMenu");

		// Ready button
		guiElements.push_back(engine->gui->addButton(core::rect<s32>(
			edgeBorderWidth - BUTTON_WIDTH,
			edgeBorderHeight - BUTTON_HEIGHT,
			edgeBorderWidth,
			edgeBorderHeight), 
			0, 
			BUTTON_READY));
		engine->lang->AddElementWithText(guiElements.back(), "Ready");

		// "Player" column label
		guiElements.push_back(engine->gui->addStaticText(L"", core::rect<s32>(
			GUI_ELEMENT_SPACING * 3,
			GUI_ELEMENT_SPACING * 2 + 7,
			GUI_ELEMENT_SPACING * 3 + 50,
			GUI_ELEMENT_SPACING * 2 + 27),
			false,
			false,
			0,
			STATIC_TEXT_PLAYER));
		engine->lang->AddElementWithText(guiElements.back(), "Player");

		// "Ready" column label
		guiElements.push_back(engine->gui->addStaticText(L"", core::rect<s32>(
			edgeBorderWidth - GUI_ELEMENT_SPACING * 5,
			GUI_ELEMENT_SPACING * 2 + 7,
			edgeBorderWidth - GUI_ELEMENT_SPACING * 2,
			GUI_ELEMENT_SPACING * 2 + 27),
			false,
			false,
			0,
			STATIC_TEXT_READY));
		engine->lang->AddElementWithText(guiElements.back(), "Ready");

		// actual list of current players in lobby
		playersGUI = engine->gui->addStaticText(L"", core::rect<s32>(
			GUI_ELEMENT_SPACING * 3,
			GUI_ELEMENT_SPACING * 4,
			GUI_ELEMENT_SPACING * 3 + 200,
			edgeBorderHeight - BUTTON_HEIGHT - GUI_ELEMENT_SPACING),
			false,
			true,
			0,
			STATIC_TEXT_PLAYER_LIST);
		guiElements.push_back(playersGUI);

		for(float i = 2.5f; i < 2.5f + MAX_PLAYERS; ++i)
		{
			auto readyImg = engine->gui->addStaticText(L"",
				core::rect<s32>(
				static_cast<int>(edgeBorderWidth - GUI_ELEMENT_SPACING * 4.5),
				static_cast<int>(CHECKBOX_SPACING * i),
				static_cast<int>(edgeBorderWidth - GUI_ELEMENT_SPACING * 3.5),
				static_cast<int>(CHECKBOX_SPACING * i + GUI_ELEMENT_SPACING) 
				),
				true,
				true,
				nullptr,
				-1,
				true);
			readyImg->setBackgroundColor(GREEN);
			readyImages.push_back(readyImg);
			guiElements.push_back(readyImg);
		}
	}

	Lobby::~Lobby()
	{
	}

	void Lobby::AddPlayer(const std::wstring &name, const int perfScore, const unsigned long location)
	{
		// make sure we don't bump a real player from the lobby
		if(!numBots)
		{
			return;
		}

		// get open AI spot in player list.  we know it is an AI b/c numBots is positive
		auto iter = players.end() - 1;
		assert(iter != players.end() && iter->type == AI);

		wcout << name;
		cout << " being added.\n";
		iter->name = name;
		iter->perfScore = perfScore;
		iter->type = HUMAN;
		iter->ready = false;
		iter->refreshTime = 0;
#ifdef WIN32
		iter->sourceAddr.sin_addr.S_un.S_addr = location;
#else
		iter->sourceAddr.sin_addr.s_addr = location;
#endif
		numBots--;
		CheckNewServerCandidate(*iter);
		sort(players.begin(), players.end(), [](const LobbyPlayer& lhs, const LobbyPlayer& rhs)
		{
			return lhs.type < rhs.type; 
		});

		UpdatePlayersOnScreen();
	}

	Network* Lobby::GetNetwork()
	{
		return network.get();
	}

	bool Lobby::OnEvent(const irr::SEvent &event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			switch(event.GUIEvent.EventType)
			{
			case gui::EGET_BUTTON_CLICKED:
				switch(id)
				{
				case BUTTON_RETURN_MENU:
					engine->eventQueue.push(FSM::MAINMENU);
					return true;

				case BUTTON_READY:
					ChangePlayerReady(network->GetIP());
					return true;
				}
				break;

			case gui::EGET_COMBO_BOX_CHANGED:
				// if another combo box is added, IDs will have to be passed out
				// and this case statement will have to include a switch
				engine->lang->ChangeLanguage(static_cast<LANG>(engine->lang->langSelector->getSelected()));
				return true;

			default:
				break;
			}
		}

		return false;

	}

	void Lobby::RemovePlayer(const unsigned long addr)
	{
		auto iter = find_if(players.begin(), players.end(), [=](LobbyPlayer p) { return GetNetworkIP(p.sourceAddr) == addr; });
		if(iter != players.end())
		{
			// setting other fields is unnecessary since we can ignore them after seeing
			// that this is a bot
			wcout << iter->name;
			wcout << " being removed.\n";
			iter->name = L"Bot";
			iter->type = AI;
			if(GetNetworkIP(iter->sourceAddr) == GetNetworkIP(designatedServer.sourceAddr))
			{
				designatedServer = LobbyPlayer();
				for_each(players.begin(), players.end(), [=](const LobbyPlayer &p){CheckNewServerCandidate(p);});
			}
			memset(&iter->sourceAddr, 0, sizeof iter->sourceAddr);
			iter->refreshTime = 0;
			iter->ready = true;
			numBots++;
			ChangePlayerReady(addr, 'T');
			UpdatePlayersOnScreen();
		}
	}

	void Lobby::Run()
	{
		auto currentTime = engine->device->getTimer()->getTime();
		if((currentTime - prevTime) > BROADCAST_INTERVAL)
		{
			BOOST_FOREACH(LobbyPlayer &p, players)
			{
				p.refreshTime += currentTime - prevTime;
			}
			prevTime = currentTime;
			PruneDisconnects();
			BroadcastPlayerInfoStatus();
		}

		auto recvMessage = network->ReceiveMessage();

		if(recvMessage.prefix == 'N')
		{
			return;
		}

		auto playerSearch = [=](const LobbyPlayer &p)
		{
			return GetNetworkIP(recvMessage.address) == GetNetworkIP(p.sourceAddr);
		};

		// find first player with a matching IP
		auto iter = find_if(players.begin(), players.end(), playerSearch);
		if(recvMessage.prefix == 'D')
		{
			// if no player was found with a matching IP, add them
			if(iter == players.end())
			{
				boost::char_separator<char> separator(" ");
				boost::tokenizer<boost::char_separator<char>> tokens(recvMessage.msg, separator);
				auto iterator = tokens.begin();
				std::wstring newPlayerName = ConvertStrToWide(*iterator);
				iterator++;
				int perf = boost::lexical_cast<int>(*iterator);
				wcout << newPlayerName;
				cout << " about to be added.\n";
				AddPlayer(newPlayerName, perf, GetNetworkIP(recvMessage.address));
			}
			else
			{
				iter->refreshTime = 0;
			}
		}
		else if(recvMessage.prefix == 'R')
		{
			// if a player with a matching IP was found, toggle ready state
			if(iter != players.end())
			{
				ChangePlayerReady(GetNetworkIP(iter->sourceAddr), recvMessage.msg[0]);
			}
		}

		if(ReadyToPlay())
		{
			// all players are ready, so make a note of the server address and
			// post event to switch to game state
			engine->serverIP = designatedServer.sourceAddr;
			engine->eventQueue.push(FSM::GAME);
		}
	}
}