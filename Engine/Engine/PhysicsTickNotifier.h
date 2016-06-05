#ifndef __PHYSICS_TICK_NOTIFIER_H__
#define __PHYSICS_TICK_NOTIFIER_H__

#include <Engine\Engine\IPhysicsTickListener.h>

#include <vector>

namespace Engine
{
	class PhysicsTickNotifier
	{
		public:

			static void CreatePhysicsTickNotifier();
			static void DestroyPhysicsTickNotifier();
			static void UpdatePhysicsTick(double deltaTime);

			static void RegisterPhysicsTick(IPhysicsTickListener* physicsTickListner);
			static void DeRegisterPhysicsTick(IPhysicsTickListener* physicsTickListner);

		private:

			std::vector<IPhysicsTickListener*> _list;
			static PhysicsTickNotifier* _instance;

			PhysicsTickNotifier();
			~PhysicsTickNotifier();
			void notifyPhysicsTick(double deltaTime);
	};
}

#endif	//__PHYSICS_TICK_NOTIFIER_H__