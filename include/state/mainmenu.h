#ifndef MAINMENU_H
#define MAINMENU_H

#include "irrlicht/irrlicht.h"
#include "state/fsmstate.h"

namespace Typhon
{
	class MainMenu : public FSMState
	{
	private:



	public:

		MainMenu(std::shared_ptr<Engine> engine);
		virtual ~MainMenu();

		virtual void Run();
	};
}

#endif