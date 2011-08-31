#include "state/lobby.h"

#include "engine/engine.h"
#include "utility/stateexception.h"

using namespace irr;

namespace Typhon
{
	// GUI ENUMS
	enum GUI_ID { 
		TABCONTROL_PLAYERPANE,

		BUTTON_READY,
		BUTTON_RETURN_MENU };

		Lobby::Lobby(std::shared_ptr<Engine> engine)
			: FSMState(engine), network(Typhon::GetNetwork(Typhon::RAW, PORT_NUMBER))		
		{
			if (!network)
			{
				throw StateException("Error starting up network code (could not allocate or incompatible system).\n");
			}

			// player pane (holds list of human players/AI who will be playing them game)
			guiElements.push_back(engine->gui->addTabControl(core::rect<irr::s32>(
				GUI_ELEMENT_SPACING * 2,
				GUI_ELEMENT_SPACING * 2,
				edgeBorderWidth,
				edgeBorderHeight - BUTTON_HEIGHT - GUI_ELEMENT_SPACING),
				0,
				false,
				true,
				TABCONTROL_PLAYERPANE));

			// Return to main menu button
			guiElements.push_back(engine->gui->addButton(core::rect<irr::s32>(
				edgeBorderWidth - BUTTON_WIDTH * 2 - GUI_ELEMENT_SPACING,
				edgeBorderHeight - BUTTON_HEIGHT,
				edgeBorderWidth - BUTTON_WIDTH - GUI_ELEMENT_SPACING,
				edgeBorderHeight), 
				0, 
				BUTTON_RETURN_MENU)); 
			engine->lang->AddElementWithText(guiElements.back(), "BackToMainMenu");

			// Player ready button
			guiElements.push_back(engine->gui->addButton(core::rect<irr::s32>(
				edgeBorderWidth - BUTTON_WIDTH,
				edgeBorderHeight - BUTTON_HEIGHT,
				edgeBorderWidth,
				edgeBorderHeight), 
				0, 
				BUTTON_READY));
			engine->lang->AddElementWithText(guiElements.back(), "Ready");
		}

		Lobby::~Lobby()
		{
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
						engine->eventQueue.push(FSM::GAME);
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

		void Lobby::Run()
		{
		}
}