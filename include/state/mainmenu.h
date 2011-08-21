#ifndef MAINMENU_H
#define MAINMENU_H

#include <vector>

#include "state/fsmstate.h"

namespace Typhon
{
	class MainMenu : public FSMState
	{
	private:

		std::vector<irr::gui::IGUIButton*> guiElements;

	public:

		MainMenu(std::shared_ptr<Engine> engine);
		virtual ~MainMenu();

		virtual bool OnEvent(const irr::SEvent &event);
		virtual void Run();
	};
}

#endif