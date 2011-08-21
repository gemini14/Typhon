#include "state/mainmenu.h"

#include "engine/engine.h"

using namespace irr;

namespace Typhon
{
	MainMenu::MainMenu(std::shared_ptr<Engine> engine)
		: FSMState(engine)
	{
	}

	MainMenu::~MainMenu()
	{
	}

	void MainMenu::Run()
	{
		// get text from lang file
	}
}