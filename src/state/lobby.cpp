#include "state/lobby.h"

#include <algorithm>
#include <regex>
#include <sstream>

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

	// GUI ENUMS
	enum GUI_ID { 
		TABCONTROL_PLAYERPANE,

		BUTTON_READY,
		BUTTON_RETURN_MENU,

		STATIC_TEXT_PLAYER,
		STATIC_TEXT_PLAYER_LIST,
		STATIC_TEXT_READY
	};

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

				auto readyBox = readyImages[i];
				if(players[i].ready)
				{
					readyBox->setBackgroundColor(GREEN);
				}
				else
				{
					readyBox->setBackgroundColor(RED);
				}
				break;
			}
		}
	}

	void Lobby::UpdatePlayersOnScreen()
	{
		std::wstringstream playerText;
		// tried to use std::copy and ostream_iterator, but it was such a pain
		// to (try to) get it working that I just went with a simple for loop to save time
		for(auto iter = players.begin(); iter != players.end(); ++iter)
		{
			playerText << iter->name << L"\n\n";
		}
		playersGUI->setText(playerText.str().c_str());
	}

	Lobby::Lobby(std::shared_ptr<Engine> engine)
		: FSMState(engine), network(Typhon::GetNetwork(Typhon::RAW, PORT_NUMBER)),
		numBots(MAX_PLAYERS), players(MAX_PLAYERS, Player()),
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

		auto iter = find_if(players.rbegin(), players.rend(), [](Player p){ return p.type == Lobby::AI; });
		if(iter != players.rend())
		{
			iter->name = name;
			iter->perfScore = perfScore;
			iter->type = HUMAN;
			iter->ready = false;
#ifdef WIN32
			iter->sourceAddr.sin_addr.S_un.S_addr = location;
#else
			// TODO: Add Linux equivalent
#endif
			numBots--;
			sort(players.begin(), players.end(), [](const Player& lhs, const Player& rhs){ return lhs.type < rhs.type; });
			UpdatePlayersOnScreen();
		}		
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

	void Lobby::RemovePlayer(const std::wstring &name)
	{
		auto iter = find_if(players.begin(), players.end(), [=](Player p) { return p.name == name; });
		if(iter != players.end())
		{
			// setting other fields is unnecessary since we can ignore them after seeing
			// that this is a bot
			iter->name = L"Bot";
			iter->type = AI;
			iter->ready = true;
			UpdatePlayersOnScreen();
		}
	}

	void Lobby::Run()
	{
		auto currentTime = engine->device->getTimer()->getTime();
		if((currentTime - prevTime) * 0.001f > 0.5f)
		{
			prevTime = currentTime;
			network->BroadcastMessage(discoveryMessage, 'D');
			auto iter = find_if(players.begin(), players.end(), [=](const Player &p)
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

			while(true)
			{
				auto recvMessage = network->ReceiveMessage();

				auto playerSearch = [=](const Player &p)
				{
					return GetNetworkIP(recvMessage.address) == GetNetworkIP(p.sourceAddr);
				};
				auto iter = find_if(players.begin(), players.end(), playerSearch);

				if(recvMessage.prefix == 'N')
				{
					break;
				}
				else if(recvMessage.prefix == 'D')
				{
					if(iter == players.end())
					{
						boost::char_separator<char> separator(" ");
						boost::tokenizer<boost::char_separator<char>> tokens(recvMessage.msg, separator);
						auto iterator = tokens.begin();
						std::wstring newPlayerName = ConvertStrToWide(*iterator);
						iterator++;
						int perf = boost::lexical_cast<int>(*iterator);
						AddPlayer(newPlayerName, perf, GetNetworkIP(recvMessage.address));
					}
				}
				else if(recvMessage.prefix == 'R')
				{
					if(iter != players.end())
					{
						ChangePlayerReady(GetNetworkIP(iter->sourceAddr), recvMessage.msg[0]);
					}
				}
			}
		}
	}
}