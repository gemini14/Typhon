#include "physics/physics.h"

namespace Typhon
{
	Physics::Physics()
	{
		 btBroadphaseInterface* broadphase = new btDbvtBroadphase();
		 delete broadphase;
	}

	Physics::~Physics()
	{
	}
}