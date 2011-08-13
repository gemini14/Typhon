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

		MainMenu();
		virtual ~MainMenu();

		virtual void Run(std::shared_ptr<Engine> engine);
	};
}

#endif