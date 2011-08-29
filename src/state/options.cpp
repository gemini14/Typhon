#include "state/options.h"

#include <iostream>

#include "engine/engine.h"
#include "utility/constants.h"
#include "utility/utility.h"

using namespace irr;
using namespace std;

namespace Typhon
{
	// GUI ENUMS
	enum GUI_IDS { 
		EDITBOX_PLAYER_NAME,
		STATICTEXT_PLAYER_NAME_DESC, 
		
		BUTTON_RETURN_MENU }; 

	Options::Options(std::shared_ptr<Engine> engine)
		: FSMState(engine)
	{
		// Player name entry box description label
		guiElements.push_back(engine->gui->addStaticText(L"",
			core::rect<s32>(
			edgeBorderWidth / 2 - 200,
			edgeBorderHeight / 2 - GUI_ELEMENT_SPACING,
			edgeBorderWidth / 2 - GUI_ELEMENT_SPACING,
			edgeBorderHeight / 2
			),
			false,
			false,
			0,
			STATICTEXT_PLAYER_NAME_DESC,
			true));
		engine->lang->AddElementWithText(guiElements.back(), "PlayerName");

		// Player name entry box
		auto nameEntryBox = engine->gui->addEditBox(engine->options.name.c_str(),
			core::rect<irr::s32>(
			edgeBorderWidth / 2 + GUI_ELEMENT_SPACING,
			edgeBorderHeight / 2 - GUI_ELEMENT_SPACING,
			edgeBorderWidth / 2 + 200,
			edgeBorderHeight / 2),
			true,
			0,
			EDITBOX_PLAYER_NAME);
		nameEntryBox->setMax(25);
		guiElements.push_back(nameEntryBox);

		// Quit button
		guiElements.push_back(engine->gui->addButton(core::rect<irr::s32>(
			edgeBorderWidth - BUTTON_WIDTH,
			edgeBorderHeight - BUTTON_HEIGHT,
			edgeBorderWidth,
			edgeBorderHeight), 
			0, 
			BUTTON_RETURN_MENU));
		engine->lang->AddElementWithText(guiElements.back(), "BackToMainMenu");
	}

	Options::~Options()
	{
	}

	bool Options::OnEvent(const irr::SEvent &event)
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
					try
					{
						auto enteredName = engine->gui->getRootGUIElement()->getElementFromId(EDITBOX_PLAYER_NAME)->getText(); 
						if(engine->options.name != enteredName)
						{
							engine->options.name = enteredName;
						}
					}
					catch(luabind::error &e)
					{
						cout << "\n" << e.what() << "\n" << lua_tostring(engine->lua.luaState, -1) << "\n";
					}
					engine->eventQueue.push(FSM::MAINMENU);
					return true;

				default:
					break;
				}
				break;

			case gui::EGET_COMBO_BOX_CHANGED:
				switch(id)
				{
				case COMBOBOX_LANG_SELECTOR_ID:
					engine->lang->ChangeLanguage(static_cast<LANG>(engine->lang->langSelector->getSelected()));
					return true;

				default:
					break;
				}
				break;

			default:
				break;
			}
		}

		return false;
	}

	void Options::Run()
	{
	}
}
