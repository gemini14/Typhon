#include "engine/engine.h"

#include "utility/utility.h"

using namespace irr;
using namespace std;

namespace Typhon
{
	Engine::Engine()
		: ready(false), terminate(false), lang(new I18N(&lua)), fonts(new FontManager())
	{
		video::E_DRIVER_TYPE driverType;
#ifdef _IRR_WINDOWS_
		driverType = video::EDT_DIRECT3D9;
#elif defined(_IRR_LINUX_PLATFORM_)
		driverType = video::EDT_OPENGL;
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
#ifdef _DEBUG
			device->getFileSystem()->addFileArchive("resources/", true, false, io::EFAT_FOLDER);
#else
			// TODO: pick a better archive type
			device->getFileSystem()->addFileArchive("resources.zip", true, false, io::EFAT_ZIP);
#endif
			
			// set GUI font
			gui->getSkin()->setFont(fonts->GetTtFont(driver, "fonts/Vera.ttf", 16));
			
			// create drop down language selector
			lang->langSelector = gui->addComboBox(core::rect<s32>(10, 10, 50, 40));
			
			int numLanguages = lang->GetNumberOfLanguages();
			for(int i = 0; i < numLanguages; ++i)
			{
				lang->langSelector->addItem(ConvertStrToWide(lang->ConvertLangToString(static_cast<LANG>(i))).c_str());
			}
			lang->langSelector->setSelected(0);

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
}