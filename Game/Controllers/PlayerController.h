#ifndef __PLAYER_CONTROLLER_H__
#define __PLAYER_CONTROLLER_H__

#include <Engine\Core\Math\Vector2f.h>
#include <Engine\Engine\IKeyboardListener.h>
#include <Engine\Engine\IPhysicsTickListener.h>

namespace Game
{
	class PlayerController : public Engine::IKeyboardListener, public Engine::IPhysicsTickListener
	{
		public:

			static void CreatePlayerController();
			static PlayerController* GetPlayerController();
			static void DestroyPlayerController();

			void setPhysicsInfo( Engine::SmartPtr<Engine::PhysicsInfo>& i_physicsInfo );

			virtual void keyboardUpdate(unsigned int i_VKeyID, bool i_bDown);
			virtual void preCollisionPhysicsTick(double deltaTime);
			virtual void postCollisionPhysicsTick(double deltaTime);

		private :
			void applyForce( Engine::Math::Vector2f force );

			static PlayerController* _instance;

			PlayerController();
			~PlayerController();
	};
}

#endif	//__PLAYER_CONTROLLER_H__
