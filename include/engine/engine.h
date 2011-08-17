#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <memory>

#include <boost/noncopyable.hpp>

#include "engine/luamanager.h"
#include "i18n/i18n.h"
#include "irrlicht/irrlicht.h"

namespace Typhon
{
	class Engine : boost::noncopyable
	{
	private:

		LuaManager lua;

	public:

		bool ready;

		irr::IrrlichtDevice *device;
		irr::scene::ISceneManager *smgr;
		irr::video::IVideoDriver *driver;
		irr::gui::IGUIEnvironment *gui;

		std::shared_ptr<I18N> lang;

		Engine();
		~Engine();
	};
}

#endif