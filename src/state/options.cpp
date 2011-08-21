#include "state/options.h"

#include "state/options.h"

#include "engine/engine.h"
#include "utility/constants.h"

using namespace irr;

namespace Typhon
{
	// GUI ENUMS
	enum GUI_IDS {}; 

	Options::Options(std::shared_ptr<Engine> engine)
		: FSMState(engine)
	{
		auto scrSize = engine->driver->getScreenSize();
		int edgeBorderWidth = scrSize.Width - GUI_SCREEN_PADDING_LARGE;
		int edgeBorderHeight = scrSize.Height - GUI_SCREEN_PADDING_LARGE;
		
		// Lobby button
		//engine->gui->addButton(core::rect<irr::s32>(
		//	edgeBorderWidth - BUTTON_WIDTH * 3 - GUI_ELEMENT_SPACING * 2,
		//	edgeBorderHeight - BUTTON_HEIGHT,
		//	edgeBorderWidth - BUTTON_WIDTH * 2 - GUI_ELEMENT_SPACING * 2,
		//	edgeBorderHeight), 0, BUTTON_ENTER_LOBBY, engine->lang->GetText(engine->lang->language, "EnterLobby").c_str());
		
	}
	
	Options::~Options()
	{
		if(!engine->terminate)
		{
			auto guiList = engine->gui->getRootGUIElement()->getChildren();
			while(!guiList.empty())
			{
				(*guiList.getLast())->remove();
			}
		}
	}

	bool Options::OnEvent(const irr::SEvent &event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			if(event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED)
			{
				s32 id = event.GUIEvent.Caller->getID();

				switch(id)
				{
				// case BUTTON_ENTER_LOBBY:
					// engine->eventQueue.push(FSM::LOBBY);
					// return true;

				default:
					return false;
				}
			}
		}

		return false;

	}

	void Options::Run()
	{
	}
}