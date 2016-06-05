#include <Engine\Engine\AIUpdateNotifier.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>

namespace Engine
{
	AIUpdateNotifier* AIUpdateNotifier::_instance = nullptr;

	void AIUpdateNotifier::CreateAIUpdateNotifier()
	{
		if (_instance == nullptr)
		{
			_instance = new AIUpdateNotifier();
		}
		else
		{
			DEBUG_PRINT("Cannot create a second instance of AIUpdateNotifier.\n");
		}
	}

	void AIUpdateNotifier::DestroyAIUpdateNotifier()
	{
		MessagedAssert(_instance != nullptr, "AIUpdateNotifier instance is null.\n");

		delete _instance;
		_instance = nullptr;
	}

	void AIUpdateNotifier::RegisterAIUpdate(IAIUpdateListener* i_AITickListner)
	{
		MessagedAssert(i_AITickListner != nullptr, "Null pointer is passed.\n");

		_instance->_list.push_back(i_AITickListner);
	}

	void AIUpdateNotifier::DeRegisterAIUpdate(IAIUpdateListener* i_AITickListner)
	{
		std::vector<IAIUpdateListener*>::iterator itr = _instance->_list.begin();

		itr = std::find(_instance->_list.begin(), _instance->_list.end(), i_AITickListner);

		if (itr != _instance->_list.end())
		{
			_instance->_list.erase(itr);
		}
		else
		{
			MessagedAssert(false, "IPhysicsTickListener not found.\n");
		}
	}

	AIUpdateNotifier::AIUpdateNotifier()
	{
	}

	AIUpdateNotifier::~AIUpdateNotifier()
	{
		_list.clear();
	}

	void AIUpdateNotifier::UpdateAITick(double i_deltaTime)
	{
		std::vector<IAIUpdateListener*>::iterator itr = _instance->_list.begin();

		for (; itr != _instance->_list.end(); itr++)
		{
			(*itr)->aiUpdate(i_deltaTime);
		}
	}
}