#include <Engine\Engine\PhysicsInfo.h>
#include <Engine\Engine\HitResult.h>
#include <stdlib.h>

namespace Engine
{
	PhysicsInfo::PhysicsInfo(float i_mass, float i_drag, Engine::SmartPtr< Engine::GameObject >& i_gameObject, const Math::Vector2f& dimension)
		:mass(i_mass),
		drag(i_drag),
		_gameObject(i_gameObject),
		colliderTag(nullptr),
		collidesWith(nullptr),
		numCollidesWith(0)
	{
		potentialPos = _gameObject->pos;
		Math::Vector2f temp;

		// right bottom
		temp.x = float(dimension.x / 2.0f);
		temp.y = -float(dimension.y / 2.0f);
		boundingBox.push_back(temp);

		// right top
		temp.x = float(dimension.x / 2.0f);
		temp.y = float(dimension.y / 2.0f);
		boundingBox.push_back(temp);

		// left top
		temp.x = -float(dimension.x / 2.0f);
		temp.y = float(dimension.y / 2.0f);
		boundingBox.push_back(temp);

		// left bottom
		temp.x = -float(dimension.x / 2.0f);
		temp.y = -float(dimension.y / 2.0f);
		boundingBox.push_back(temp);

		hitResult = new Collision::HitResult();
	}

	PhysicsInfo::~PhysicsInfo()
	{
		free(collidesWith);
		collidesWith = nullptr;
		delete hitResult;
	}
}