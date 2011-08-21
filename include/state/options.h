#ifndef OPTIONS_H
#define OPTIONS_H

#include "state/fsmstate.h"

namespace Typhon
{
	class Options : public FSMState
	{
	private:


	public:

		Options(std::shared_ptr<Engine> engine);
		virtual ~Options();

		virtual bool OnEvent(const irr::SEvent &event);
		virtual void Run();
	};
}

#endif