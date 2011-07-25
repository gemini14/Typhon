#ifndef ENGINE_H
#define ENGINE_H

#include <boost/noncopyable.hpp>
#include <irrlicht/irrlicht.h>

namespace Typhon
{
	class Engine : boost::noncopyable
	{
	private:

	public:

		bool ready;

		irr::IrrlichtDevice *device;
		irr::scene::ISceneManager *smgr;
		irr::video::IVideoDriver *driver;
		irr::gui::IGUIEnvironment *gui;

		Engine();
		~Engine();
	};
}

#endif