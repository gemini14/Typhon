#include "state/options.h"

#include "state/options.h"

#include "engine/engine.h"
#include "utility/constants.h"
#include "utility/utility.h"

using namespace irr;
using namespace std;

namespace Typhon
{
	// GUI ENUMS
	enum GUI_IDS { EDITBOX_PLAYER_NAME, COMBOBOX_PREF_LANG, BUTTON_RETURN_MENU }; 

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
			s32 id = event.GUIEvent.Caller->getID();
			switch(event.GUIEvent.EventType) 
			{
			case gui::EGET_BUTTON_CLICKED:
				switch(id)
				{
				case BUTTON_RETURN_MENU:
					try
					{
						luabind::call_function<void>(engine->lua.luaState, "SetOption", 
							"Name", ConvertWideToStr(engine->options.name));
						//luabind::call_function<void>(engine->lua.luaState, "SetOption",
						//	"Language", );
					}
					catch(luabind::error &e)
					{
						cout << "\n" << e.what() << "\n" << lua_tostring(engine->lua.luaState, -1) << "\n";
					}
					break;

				default:
					return false;
				}
				break;

			case gui::EGET_COMBO_BOX_CHANGED:
				switch(id)
				{
				case COMBOBOX_LANG_SELECTOR_ID:
					engine->lang->ChangeLanguage(static_cast<LANG>(engine->lang->langSelector->getSelected()));
					break;

				default:
					return false;
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