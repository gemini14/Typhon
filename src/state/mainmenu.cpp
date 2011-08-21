#include "state/mainmenu.h"

#include "engine/engine.h"
#include "utility/constants.h"

using namespace irr;

namespace Typhon
{
	// GUI ENUMS
	enum GUI_IDS { BUTTON_ENTER_LOBBY, BUTTON_OPTIONS, BUTTON_QUIT };

	MainMenu::MainMenu(std::shared_ptr<Engine> engine)
		: FSMState(engine)
	{
		auto scrSize = engine->driver->getScreenSize();
		int edgeBorderWidth = scrSize.Width - GUI_SCREEN_PADDING_LARGE;
		int edgeBorderHeight = scrSize.Height - GUI_SCREEN_PADDING_LARGE;
		
		// Lobby button
		guiElements.push_back(engine->gui->addButton(core::rect<irr::s32>(
			edgeBorderWidth - BUTTON_WIDTH * 3 - GUI_ELEMENT_SPACING * 2,
			edgeBorderHeight - BUTTON_HEIGHT,
			edgeBorderWidth - BUTTON_WIDTH * 2 - GUI_ELEMENT_SPACING * 2,
			edgeBorderHeight), 
			0, 
			BUTTON_ENTER_LOBBY, 
			engine->lang->GetText(engine->lang->language, "EnterLobby").c_str()));
		
		// Options button
		guiElements.push_back(engine->gui->addButton(core::rect<irr::s32>(
			edgeBorderWidth - BUTTON_WIDTH * 2 - GUI_ELEMENT_SPACING,
			edgeBorderHeight - BUTTON_HEIGHT,
			edgeBorderWidth - BUTTON_WIDTH - GUI_ELEMENT_SPACING,
			edgeBorderHeight), 
			0, 
			BUTTON_OPTIONS, 
			engine->lang->GetText(engine->lang->language, "Options").c_str()));

		// Quit button
		guiElements.push_back(engine->gui->addButton(core::rect<irr::s32>(
			edgeBorderWidth - BUTTON_WIDTH,
			edgeBorderHeight - BUTTON_HEIGHT,
			edgeBorderWidth,
			edgeBorderHeight), 
			0, 
			BUTTON_QUIT, 
			engine->lang->GetText(engine->lang->language, "Quit").c_str()));
	}

	MainMenu::~MainMenu()
	{
		if(!engine->terminate)
		{
			for(auto i = guiElements.begin(); i < guiElements.end(); ++i)
			{
				(*i)->remove();
			}
		}
	}

	bool MainMenu::OnEvent(const irr::SEvent &event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			if(event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED)
			{
				s32 id = event.GUIEvent.Caller->getID();

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
			}
		}

		return false;

	}

	void MainMenu::Run()
	{
		// get text from lang file
	}
}