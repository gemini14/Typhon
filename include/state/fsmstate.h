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

		FSMState()
		{
		}
		
		virtual ~FSMState()
		{
		}

		virtual void Run(std::shared_ptr<Engine> engine) = 0;
	};
}

#endif