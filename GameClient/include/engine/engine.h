#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <queue>
#include <string>

#include <boost/noncopyable.hpp>
#ifdef WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

#include "engine/luamanager.h"
#include "i18n/i18n.h"
#include "irrlicht/irrlicht.h"
#include "font/font_manager.h"
#include "state/fsmevents.h"
#include "utility/useroptions.h"

namespace Typhon
{
	class Engine : boost::noncopyable
	{
	private:

		std::unique_ptr<FontManager> fonts;

	public:

		bool ready;
		bool terminate;

		int perfScore;

		irr::IrrlichtDevice *device;
		irr::scene::ISceneManager *smgr;
		irr::video::IVideoDriver *driver;
		irr::gui::IGUIEnvironment *gui;

		LuaManager lua;
		std::queue<FSM::EVENT_TYPE> eventQueue;
		std::shared_ptr<I18N> lang;

		UserOptions options;
		sockaddr_in serverIP;
		bool clientIsServer;
		std::shared_ptr<std::string> lobbyList;

		Engine();
		~Engine();

		void SavePrefs();
	};
}

#endif
