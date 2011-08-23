#include "state/mainmenu.h"

#include "engine/engine.h"

using namespace irr;

namespace Typhon
{
	// GUI ENUMS
	enum GUI_ID { BUTTON_ENTER_LOBBY, BUTTON_OPTIONS, BUTTON_QUIT };

	MainMenu::MainMenu(std::shared_ptr<Engine> engine)
		: FSMState(engine)
	{
		// Lobby button
		
		guiElements.push_back(engine->gui->addButton(core::rect<irr::s32>(
			edgeBorderWidth - BUTTON_WIDTH * 3 - GUI_ELEMENT_SPACING * 2,
			edgeBorderHeight - BUTTON_HEIGHT,
			edgeBorderWidth - BUTTON_WIDTH * 2 - GUI_ELEMENT_SPACING * 2,
			edgeBorderHeight), 
			0, 
			BUTTON_ENTER_LOBBY));
		engine->lang->AddElementWithText(guiElements.back(), "EnterLobby");
					
		// Options button
		guiElements.push_back(engine->gui->addButton(core::rect<irr::s32>(
			edgeBorderWidth - BUTTON_WIDTH * 2 - GUI_ELEMENT_SPACING,
			edgeBorderHeight - BUTTON_HEIGHT,
			edgeBorderWidth - BUTTON_WIDTH - GUI_ELEMENT_SPACING,
			edgeBorderHeight), 
			0, 
			BUTTON_OPTIONS)); 
		engine->lang->AddElementWithText(guiElements.back(), "Options");

		// Quit button
		guiElements.push_back(engine->gui->addButton(core::rect<irr::s32>(
			edgeBorderWidth - BUTTON_WIDTH,
			edgeBorderHeight - BUTTON_HEIGHT,
			edgeBorderWidth,
			edgeBorderHeight), 
			0, 
			BUTTON_QUIT));
		engine->lang->AddElementWithText(guiElements.back(), "Quit");
	}

	MainMenu::~MainMenu()
	{
	}

	bool MainMenu::OnEvent(const irr::SEvent &event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			switch(event.GUIEvent.EventType)
			{
			case gui::EGET_BUTTON_CLICKED:
				switch(id)
				{
				case BUTTON_ENTER_LOBBY:
					engine->eventQueue.push(FSM::LOBBY);
					return true;

				case BUTTON_OPTIONS:
					engine->eventQueue.push(FSM::OPTIONS);
					return true;

				case BUTTON_QUIT:
					engine->terminate = true;
					return true;

				default:
					return false;
				}
				break;

			case gui::EGET_COMBO_BOX_CHANGED:
				// if another combo box is added, IDs will have to be passed out
				// and this case statement will have to include a switch
				engine->lang->ChangeLanguage(static_cast<LANG>(engine->lang->langSelector->getSelected()));
				break;

			default:
				break;
			}
		}

		return false;

	}

	void MainMenu::Run()
	{
		// get text from lang file
	}
}