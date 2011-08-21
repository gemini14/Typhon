#ifndef MAINMENU_H
#define MAINMENU_H

#include "irrlicht/irrlicht.h"
#include "state/fsmstate.h"

namespace Typhon
{
	class MainMenu : public FSMState, irr::IEventReceiver
	{
	private:



	public:

		MainMenu(std::shared_ptr<Engine> engine);
		virtual ~MainMenu();

		virtual bool OnEvent(const irr::SEvent &event);
		virtual void Run();
	};
}

#endif