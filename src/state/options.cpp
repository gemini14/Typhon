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
		
	}
	
	Options::~Options()
	{
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