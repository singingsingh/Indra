#ifndef __MONSTER_CONTROLLER_H__
#define __MONSTER_CONTROLLER_H__

#include <Engine\Core\Math\Vector2f.h>

#include <Engine\Engine\IPhysicsTickListener.h>
#include <Engine\Engine\IAIUpdateListener.h>

namespace Game
{
	class MonsterController : public Engine::IPhysicsTickListener, public Engine::IAIUpdateListener
	{
		public:
			static void CreateMonsterController();
			static MonsterController* GetMonsterController();
			static void DestroyMonsterController();

			void setPhysicsInfo( Engine::SmartPtr<Engine::PhysicsInfo>& i_physicsInfo );

			virtual void preCollisionPhysicsTick(double deltaTime);
			virtual void postCollisionPhysicsTick(double deltaTime);
			virtual void aiUpdate( double deltaTime );

		private :

			void applyForce( Engine::Math::Vector2f force );
			MonsterController();
			~MonsterController();

			static MonsterController* _instance;
			float runTime;
	};
}

#endif	//__MONSTER_CONTROLLER_H__
