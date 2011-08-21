#include "state/mainmenu.h"

#include "engine/engine.h"
#include "state/machine.h"
#include "utility/constants.h"

using namespace irr;

namespace Typhon
{
	// GUI ENUMS
	enum GUI_IDS { BUTTON_ENTER_LOBBY, BUTTON_QUIT };

	MainMenu::MainMenu(std::shared_ptr<Engine> engine)
		: FSMState(engine)
	{
		auto scrSize = engine->driver->getScreenSize();
		int edgeBorderWidth = scrSize.Width - 100;
		int edgeBorderHeight = scrSize.Height - 100;

		// Quit button
		engine->gui->addButton(core::rect<irr::s32>(
			edgeBorderWidth - BUTTON_WIDTH,
			edgeBorderHeight - BUTTON_HEIGHT,
			edgeBorderWidth,
			edgeBorderHeight), 0, BUTTON_QUIT, engine->lang->GetText(engine->lang->language, "Quit").c_str());

		engine->gui->addButton(core::rect<irr::s32>(
			edgeBorderWidth - BUTTON_WIDTH * 2 - GUI_ELEMENT_SPACING,
			edgeBorderHeight - BUTTON_HEIGHT,
			edgeBorderWidth - BUTTON_WIDTH - GUI_ELEMENT_SPACING,
			edgeBorderHeight), 0, BUTTON_ENTER_LOBBY, engine->lang->GetText(engine->lang->language, "EnterLobby").c_str());
	}

	MainMenu::~MainMenu()
	{
		auto guiList = engine->gui->getRootGUIElement()->getChildren();
		while(!guiList.empty())
		{
			(*guiList.getLast())->remove();
		}
	}

	bool MainMenu::OnEvent(const irr::SEvent &event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			if(event.GUIEvent.EventType == gui::EGUI_EVENT_TYPE::EGET_BUTTON_CLICKED)
			{
				s32 id = event.GUIEvent.Caller->getID();

				switch(id)
				{
				case BUTTON_QUIT:

					return true;

				case BUTTON_ENTER_LOBBY:

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