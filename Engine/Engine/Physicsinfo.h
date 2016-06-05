#ifndef __PHYSICS_INFO_H__
#define __PHYSICS_INFO_H__

#include <Engine\Core\Math\Vector2f.h>
#include <Engine\Core\SmartPtr.h>
#include <Engine\Engine\GameObject.h>

#include <vector>

namespace Engine
{
	namespace Collision
	{
		struct HitResult;
	}

	class PhysicsInfo
	{
		public :

			PhysicsInfo(float mass, float drag, Engine::SmartPtr< Engine::GameObject >& gameObject, const Math::Vector2f& dimensions);
			~PhysicsInfo();
			
			Math::Vector2f velocity;
			Math::Vector2f acceleration;
			Math::Vector2f potentialPos;
			float mass;
			float drag;
			const char * colliderTag;
			const char ** collidesWith;
			unsigned short numCollidesWith;
			Collision::HitResult* hitResult;

			std::vector<Math::Vector2f> boundingBox;
			Engine::SmartPtr< Engine::GameObject > _gameObject;
	};
}

#endif	__PHYSICS_INFO_H__
