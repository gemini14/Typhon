#include "engine/engine.h"

using namespace irr;
using namespace std;

namespace Typhon
{
	Engine::Engine()
		: ready(false), lang(new I18N(&lua))
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