#include <Game\Controllers\MonsterController.h>

#include <Engine\Util\Assert.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Engine\PhysicsTickNotifier.h>
#include <Engine\Engine\AIUpdateNotifier.h>
#include <Engine\Engine\HitResult.h>
#include <Engine\Core\Math\Vector2f.h>

#include <time.h>

extern bool bQuit;

namespace Game
{
	MonsterController* MonsterController::_instance = nullptr;

	MonsterController::MonsterController()
	{
		Engine::PhysicsTickNotifier::RegisterPhysicsTick(this);
		Engine::AIUpdateNotifier::RegisterAIUpdate(this);

		runTime = 0.0f;
		srand((unsigned int)time(NULL));
	}

	MonsterController::~MonsterController()
	{
		Engine::PhysicsTickNotifier::DeRegisterPhysicsTick(this);
		Engine::AIUpdateNotifier::DeRegisterAIUpdate(this);
	}

	void MonsterController::CreateMonsterController()
	{
		if (_instance == nullptr)
		{
			_instance = new Game::MonsterController();
		}
		else
		{
			DEBUG_PRINT("Cannot create two instances of Player controller.\n");
		}
	}

	MonsterController* MonsterController::GetMonsterController()
	{
		return _instance;
	}

	void MonsterController::DestroyMonsterController()
	{
		if (_instance)
		{
			delete _instance;
			_instance = nullptr;
		}
	}

	void MonsterController::setPhysicsInfo(Engine::SmartPtr<Engine::PhysicsInfo>& i_physicsInfo)
	{
		MessagedAssert(i_physicsInfo != nullptr, "Cannot set empty PhysicsInfo.\n");
		_physicsInfo = i_physicsInfo;
	}

	void MonsterController::aiUpdate(double deltaTime)
	{
		runTime -= (float)deltaTime;

		if (runTime < 0)
		{
			runTime = 3000; 

			Engine::Math::Vector2f force;
			force.x = ((float)(rand() % 2) / 50) + ((float)(rand() % 2) / 30);
			force.y = ((float)(rand() % 2) / 30) + ((float)(rand() % 2) / 50);

			if (rand() % 2 == 0)
			{
				force.x = -force.x;
			}

			if (rand() % 2 == 0)
			{
				force.y = -force.y; 
			}

			Engine::Math::Vector2f offset;
			offset.x = (float)(rand() % 20) + (float)(rand() % 23);
			offset.y = (float)(rand() % 12) + (float)(rand() % 30);

			if (rand() % 2 == 0)
			{
				offset.x = -offset.x;
			}

			if (rand() % 2 == 0)
			{
				offset.y = -offset.y;
			}

			_physicsInfo->_gameObject->pos += offset;

			Engine::Math::Vector2f distance = _physicsInfo->_gameObject->pos;
			if (distance.length() > 300.0f)
			{
				distance.normalize();
				distance = distance * 0.02f;
				distance.x = -distance.x;
				distance.y = -distance.y;
				applyForce(distance);
			}
			else
			{
				applyForce(force);
			}
		}
	}

	void MonsterController::preCollisionPhysicsTick(double i_deltaTime)
	{
		_physicsInfo->acceleration *= _physicsInfo->drag;
		Engine::Math::Vector2f newVelocity = _physicsInfo->acceleration * (float)i_deltaTime;
		_physicsInfo->potentialPos = _physicsInfo->_gameObject->pos;

		_physicsInfo->potentialPos += (_physicsInfo->velocity + newVelocity) * 0.5f * (float)i_deltaTime;
		_physicsInfo->velocity = newVelocity;
	}

	void MonsterController::postCollisionPhysicsTick(double i_deltaTime)
	{
		//law of conservation of momemtum

		if (_physicsInfo->hitResult->collision)
		{
			bQuit = true;

			float m1 = _physicsInfo->mass;
			float m2 = _physicsInfo->hitResult->other->mass;
			Engine::Math::Vector2f v1 = _physicsInfo->velocity;
			Engine::Math::Vector2f v2 = _physicsInfo->hitResult->other->velocity;

			_physicsInfo->velocity = (((m1 - m2) / (m1 + m2)) * v1) + (((2 * m2) / (m1 + m2)) * v2);
			_physicsInfo->acceleration = _physicsInfo->velocity / (float)i_deltaTime;
			_physicsInfo->_gameObject->pos += _physicsInfo->velocity*(float)i_deltaTime;

			_physicsInfo->hitResult->collision = false;
		}
		else
		{
			_physicsInfo->_gameObject->pos = _physicsInfo->potentialPos;
		}

		// clean up
		_physicsInfo->hitResult->collision = false;
		_physicsInfo->hitResult->other = nullptr;
	}

	void MonsterController::applyForce( Engine::Math::Vector2f i_force )
	{
		_physicsInfo->acceleration += i_force / _physicsInfo->mass;
	}
}