#ifndef FSMSTATE_H
#define FSMSTATE_H

#include <memory>

#include <boost/noncopyable.hpp>

#include "engine/engine.h"
#include "irrlicht/irrlicht.h"
#include "utility/constants.h"

namespace Typhon
{
	class FSMState : boost::noncopyable, public irr::IEventReceiver
	{
	protected:

		std::vector<irr::gui::IGUIButton*> guiElements;
		int edgeBorderWidth;
		int edgeBorderHeight;

	public:

		std::shared_ptr<Engine> engine;

		FSMState(std::shared_ptr<Engine> engine)
			: engine(engine)
		{
			engine->device->setEventReceiver(this);

			auto scrSize = engine->driver->getScreenSize();
			edgeBorderWidth = scrSize.Width - GUI_SCREEN_PADDING_LARGE;
			edgeBorderHeight = scrSize.Height - GUI_SCREEN_PADDING_LARGE;
		}

		virtual ~FSMState()
		{
			if(!engine->terminate)
			{
				engine->lang->ClearAllElements();

				for(auto i = guiElements.begin(); i < guiElements.end(); ++i)
				{
					(*i)->remove();
				}
			}
		}

		virtual bool OnEvent(const irr::SEvent &event) = 0;
		virtual void Run() = 0;
	};
}

#endif