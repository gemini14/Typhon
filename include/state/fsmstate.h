#ifndef FSMSTATE_H
#define FSMSTATE_H

#include <memory>

#include <boost/noncopyable.hpp>

#include "engine/engine.h"
#include "irrlicht/irrlicht.h"

namespace Typhon
{
	class FSMState : boost::noncopyable, public irr::IEventReceiver
	{
	public:

		std::shared_ptr<Engine> engine;

		FSMState(std::shared_ptr<Engine> engine)
			: engine(engine)
		{
			engine->device->setEventReceiver(this);
		}
		
		virtual ~FSMState()
		{
		}

		virtual bool OnEvent(const irr::SEvent &event) = 0;
		virtual void Run() = 0;
	};
}

#endif