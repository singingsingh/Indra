#include <Engine\Engine\PhysicsTickNotifier.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>
#include <Engine\Engine\Collision.h>

namespace Engine
{
	PhysicsTickNotifier* PhysicsTickNotifier::_instance = nullptr;

	void PhysicsTickNotifier::CreatePhysicsTickNotifier()
	{
		if (_instance == nullptr)
		{
			_instance = new PhysicsTickNotifier();
		}
		else
		{
			DEBUG_PRINT("Cannot create a second instance of PhysicsTickNotifier.\n");
		}
	}

	void PhysicsTickNotifier::DestroyPhysicsTickNotifier()
	{
		MessagedAssert(_instance!=nullptr, "PhysicsTickNotifier instance is null.\n");

		delete _instance;
		_instance = nullptr;
	}

	void PhysicsTickNotifier::UpdatePhysicsTick(double i_deltaTime)
	{
		_instance->notifyPhysicsTick(i_deltaTime);
	}

	void PhysicsTickNotifier::RegisterPhysicsTick(IPhysicsTickListener* i_PhysicsTickListner)
	{
		MessagedAssert(i_PhysicsTickListner !=nullptr, "Null pointer is passed.\n");

		_instance->_list.push_back(i_PhysicsTickListner);
	}

	void PhysicsTickNotifier::DeRegisterPhysicsTick(IPhysicsTickListener* i_PhysicsTickListner)
	{
		std::vector<IPhysicsTickListener*>::iterator itr = _instance->_list.begin();

		itr = std::find(_instance->_list.begin(), _instance->_list.end(), i_PhysicsTickListner);

		if (itr != _instance->_list.end())
		{
			_instance->_list.erase(itr);
		}
		else
		{
			MessagedAssert(false, "IPhysicsTickListener not found.\n");
		}
	}

	PhysicsTickNotifier::PhysicsTickNotifier()
	{
	}

	PhysicsTickNotifier::~PhysicsTickNotifier()
	{
		_list.clear();
	}

	void PhysicsTickNotifier::notifyPhysicsTick(double i_deltaTime)
	{
		if (_list.size() == 0)
		{
			return;
		}

		// calculate precollision potential position 
		std::vector<IPhysicsTickListener*>::iterator itr = _list.begin();

		for (; itr != _list.end(); itr++)
		{
			(*itr)->preCollisionPhysicsTick(i_deltaTime);
		}

		// calculate collisions
		std::vector<IPhysicsTickListener*>::iterator me = _list.begin();
		std::vector<IPhysicsTickListener*>::iterator other;
		const char** meCollidesWith = nullptr;
		const char* otherColliderTag = nullptr;

		for (; me != _list.end()-1; me++)
		{
			other = me + 1;
			for (; other != _list.end(); other++)
			{
				// check for the tag if it can collide.
				otherColliderTag = (*other)->_physicsInfo->colliderTag;
				meCollidesWith = (*me)->_physicsInfo->collidesWith;

				for (int i = 0; i < (*me)->_physicsInfo->numCollidesWith; i++)
				{
					if ((*meCollidesWith) == otherColliderTag)
					{
						// check for collision
						Collision::CheckCollision( *(me), *(other) , (float)i_deltaTime);
						break;
					}
					meCollidesWith++;
				}
			}
		}

		// move the GameObjects based on collisions
		itr = _list.begin();
		for (; itr != _list.end(); itr++)
		{
			(*itr)->postCollisionPhysicsTick(i_deltaTime);
		}
	}
}