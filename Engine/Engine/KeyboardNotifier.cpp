#include <Engine\Engine\KeyboardNotifier.h>

#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>

#include <Engine\System\Keyboard.h>

namespace Engine
{
	KeyboardNotifier* KeyboardNotifier::_instance = nullptr;

	void _keyboardUpdate(unsigned int i_VKeyID, bool i_bDown)
	{
		KeyboardNotifier::_instance->notifyKeyboardUpdate(i_VKeyID, i_bDown);
	}

	void KeyboardNotifier::notifyKeyboardUpdate(unsigned int i_VKeyID, bool i_bDown)
	{
		std::vector<IKeyboardListener*>::iterator itr = _list.begin();

		for (; itr != _list.end(); itr++)
		{
			(*itr)->keyboardUpdate(i_VKeyID, i_bDown);
		}
	}

	void KeyboardNotifier::CreateKeyboardNotifier()
	{
		if (_instance == nullptr)
		{
			_instance = new KeyboardNotifier();
		}
		else
		{
			DEBUG_PRINT("Cannot create a second instance of KeyboardNotifier.\n");
		}
	}

	void KeyboardNotifier::DestroyKeyboardNotifier()
	{
		MessagedAssert(_instance!=nullptr, "KeyboardNotifier instance is null.\n");

		delete _instance;
		_instance = nullptr;
	}

	void KeyboardNotifier::RegisterKeyboardUpdate(IKeyboardListener* i_keyboardListner)
	{
		MessagedAssert(i_keyboardListner !=nullptr, "Null Pointer is passed.\n");
		_instance->_list.push_back(i_keyboardListner);
	}

	void KeyboardNotifier::DeRegisterKeyboardUpdate(IKeyboardListener* i_keyboardListner)
	{
		std::vector<IKeyboardListener*>::iterator itr = _instance->_list.begin();

		itr = std::find(_instance->_list.begin(), _instance->_list.end(), i_keyboardListner);

		if (itr != _instance->_list.end())
		{
			_instance->_list.erase(itr);
		}
		else
		{
			MessagedAssert(false, "IKeyboardListener not found.\n");
		}
	}

	KeyboardNotifier::KeyboardNotifier()
	{
		SetKeyStateChangeCallback(&_keyboardUpdate);
	}

	KeyboardNotifier::~KeyboardNotifier()
	{
		_list.clear();
	}
}