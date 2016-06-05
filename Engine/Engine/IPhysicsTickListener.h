#ifndef __I_PHYSICS_TICK_LISTENER_H__
#define __I_PHYSICS_TICK_LISTENER_H__

#include <Engine\Engine\PhysicsInfo.h>
#include <Engine\Core\SmartPtr.h>

namespace Engine
{
	class IPhysicsTickListener
	{
		public :
			virtual void preCollisionPhysicsTick(double deltaTime) = 0;
			virtual void postCollisionPhysicsTick(double deltaTime) = 0;

			Engine::SmartPtr<Engine::PhysicsInfo> _physicsInfo;
	};
}

#endif	//__I_PHYSICS_TICK_LISTENER_H__
