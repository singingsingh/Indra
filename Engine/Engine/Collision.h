#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <Engine\Engine\IPhysicsTickListener.h>

namespace Engine
{
	namespace Collision
	{
		void CheckCollision(IPhysicsTickListener* me, IPhysicsTickListener* other, float deltaTime);
	}
}

#endif	//__COLLISION_H__
