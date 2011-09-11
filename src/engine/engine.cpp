#include "engine/engine.h"

#include <iostream>

#include "utility/constants.h"
#include "utility/utility.h"

using namespace irr;
using namespace std;

namespace Typhon
{
	Engine::Engine()
		: fonts(new FontManager()), ready(false), terminate(false), lang(new I18N(&lua))
	{
		video::E_DRIVER_TYPE driverType;
#ifdef _IRR_WINDOWS_
		driverType = video::EDT_DIRECT3D9;
#elif defined(_IRR_LINUX_PLATFORM_)
		// TODO: find way to get OpenGL working at acceptable speed (software renderer runs smoothly,
		// but at a much lower framerate)
		driverType = video::EDT_SOFTWARE;
		// driverType = video::EDT_OPENGL;
#endif
		core::dimension2d<u32> resolution(1024, 768);

		device = createDevice(driverType, resolution, 32);

#ifdef _IRR_LINUX_PLATFORM_
		if(!device)
		{
			driverType = video::EDT_SOFTWARE;
			device = createDevice(driverType, resolution, 32);
		}
#endif

		if(device)
		{
			device->setWindowCaption(L"Project Typhon");
			driver = device->getVideoDriver();
			smgr = device->getSceneManager();
			gui = device->getGUIEnvironment();

			// add resource directory/archive
#ifdef DEBUG
			device->getFileSystem()->addFileArchive("resources/", true, false, io::EFAT_FOLDER);
#else
			// TODO: pick a better archive type
			device->getFileSystem()->addFileArchive("resources.zip", true, false, io::EFAT_ZIP);
#endif

			// set GUI font
			gui->getSkin()->setFont(fonts->GetTtFont(driver, "fonts/Ubuntu-R.ttf", 16));

			// Get user options from Lua file
			try
			{
				luabind::call_function<void>(lua.luaState, "GetUserData");
				options.name = ConvertStrToWide(luabind::call_function<std::string>(lua.luaState,
					"GetOption", "Name"));
				options.language = luabind::call_function<std::string>(lua.luaState,
					"GetOption", "Language");
			}
			catch(luabind::error &e)
			{
				cout << "\n" << e.what() << "\n" << lua_tostring(lua.luaState, -1) << "\n";
				options.name = L"Player1";
				options.language = "en";
			}

			// create drop down language selector
			lang->langSelector = gui->addComboBox(core::rect<s32>(10, 10, 50, 40), 0, COMBOBOX_LANG_SELECTOR_ID);
			
			int numLanguages = lang->GetNumberOfLanguages();
			int userLangGUIid = 0;
			LANG userLangEnum = lang->ConvertStringToLang(options.language);
			if(userLangEnum == INVALID)
			{
				userLangEnum = EN;
			}

			for(int i = 0; i < numLanguages; ++i)
			{
				LANG langToAdd = static_cast<LANG>(i);
				lang->langSelector->addItem(ConvertStrToWide(lang->ConvertLangToString(langToAdd)).c_str());

				if(langToAdd == userLangEnum)
				{
					userLangGUIid = i;
				}
			}
			lang->langSelector->setSelected(userLangGUIid);
			lang->ChangeLanguage(userLangEnum);

			memset(&serverIP, 0, sizeof serverIP);

			// indicate that components have successfully been initialized
			ready = true;
		}	
	}

	Engine::~Engine()
	{
		if(device)
		{
			device->drop();
		}
	}

	void Engine::SavePrefs()
	{
		options.language = lang->ConvertLangToString(static_cast<LANG>(lang->langSelector->getSelected()));
		try
		{
			luabind::call_function<void>(lua.luaState, "SetOption", "Name", ConvertWideToStr(options.name));
			luabind::call_function<void>(lua.luaState, "SetOption", "Language", options.language);
			luabind::call_function<void>(lua.luaState, "SaveUserData");
		}
		catch(luabind::error &e)
		{
			cout << "\n" << e.what() << "\n" << lua_tostring(lua.luaState, -1) << "\n";
		}
	}
}
