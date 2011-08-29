#include "state/lobby.h"

#include "engine/engine.h"

using namespace irr;

namespace Typhon
{
		// GUI ENUMS
	enum GUI_ID { };

	Lobby::Lobby(std::shared_ptr<Engine> engine)
		: FSMState(engine)
	{
		
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