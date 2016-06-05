#include <Game\Controllers\PlayerController.h>

#include <Engine\Util\Assert.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Engine\PhysicsTickNotifier.h>
#include <Engine\Engine\HitResult.h>
#include <Engine\Engine\KeyboardNotifier.h>

namespace Game
{
	PlayerController* PlayerController::_instance = nullptr;

	PlayerController::PlayerController()
	{
		Engine::PhysicsTickNotifier::RegisterPhysicsTick(this);
		Engine::KeyboardNotifier::RegisterKeyboardUpdate(this);
	}

	PlayerController::~PlayerController()
	{
		Engine::PhysicsTickNotifier::DeRegisterPhysicsTick(this);
		Engine::KeyboardNotifier::DeRegisterKeyboardUpdate(this);
	}

	void PlayerController::CreatePlayerController()
	{
		if (_instance == nullptr)
		{
			_instance = new Game::PlayerController();
		}
		else
		{
			DEBUG_PRINT("Cannot create two instances of Player controller.\n");
		}
	}

	PlayerController* PlayerController::GetPlayerController()
	{
		return _instance;
	}

	void PlayerController::DestroyPlayerController()
	{
		if (_instance)
		{
			delete _instance;
			_instance = nullptr;
		}
	}

	void PlayerController::setPhysicsInfo(Engine::SmartPtr<Engine::PhysicsInfo>& i_physicsInfo)
	{
		MessagedAssert(i_physicsInfo != nullptr, "Cannot set empty PhysicsInfo.\n");
		_physicsInfo = i_physicsInfo;
	}

	void PlayerController::keyboardUpdate(unsigned int i_VKeyID, bool i_bDown)
	{
		MessagedAssert(_physicsInfo != nullptr, "No PhysicsInfo in player controller.\n" );

		switch (i_VKeyID)
		{
			case 0x57:	// w
				if (i_bDown)
				{
					Engine::Math::Vector2f force(0.0f, 0.05f);
					applyForce(force);
				}
				break;


			case 0x41:	// a
				if (i_bDown)
				{
					Engine::Math::Vector2f force(-0.05f, 0.0f);
					applyForce(force);
				}
				break;

			case 0x44:	// d
				if (i_bDown)
				{
					Engine::Math::Vector2f force(0.05f, 0.0f);
					applyForce(force);
				}
				break;

			case 0x53:	// s
				if (i_bDown)
				{
					Engine::Math::Vector2f force(0.0f, -0.05f);
					applyForce(force);
				}
				break;
		}
	}

	void PlayerController::preCollisionPhysicsTick(double i_deltaTime)
	{
		_physicsInfo->acceleration *= _physicsInfo->drag;
		Engine::Math::Vector2f newVelocity = _physicsInfo->acceleration * (float)i_deltaTime;
		_physicsInfo->potentialPos = _physicsInfo->_gameObject->pos;
		_physicsInfo->potentialPos += (_physicsInfo->velocity + newVelocity) * 0.5f * (float)i_deltaTime;
		_physicsInfo->velocity = newVelocity;
	}

	void PlayerController::postCollisionPhysicsTick(double i_deltaTime)
	{
		//law of conservation of momemtum

		if (_physicsInfo->hitResult->collision)
		{
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

	void PlayerController::applyForce( Engine::Math::Vector2f i_force )
	{
		if (_physicsInfo->acceleration.lengthSQ() < 0.000001)
		{
			_physicsInfo->acceleration += i_force / _physicsInfo->mass;
		}
	}
}