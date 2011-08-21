#ifndef FSMSTATE_H
#define FSMSTATE_H

#include <memory>

#include <boost/noncopyable.hpp>

namespace Typhon
{
	class Engine;

	class FSMState : boost::noncopyable
	{
	public:

		std::shared_ptr<Engine> engine;

		FSMState(std::shared_ptr<Engine> engine)
			: engine(engine)
		{
		}
		
		virtual ~FSMState()
		{
		}

		virtual void Run() = 0;
	};
}

#endif