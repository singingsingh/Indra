#ifndef __HIT_RESULT_H__
#define __HIT_RESULT_H__

#include <Engine\Core\Math\Vector2f.h>
#include <Engine\Engine\Physicsinfo.h>
#include <Engine\Core\SmartPtr.h>

namespace Engine
{
	namespace Collision
	{
		struct HitResult
		{
			bool collision;
			Math::Vector2f impactNormal;
			Engine::SmartPtr< Engine::PhysicsInfo > other;

			HitResult();
		};
	}
}	// namespace Engine

#endif	//__HIT_RESULT_H__
