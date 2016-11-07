#include <Engine\Engine\KeyboardNotifier.h>

#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>

#include <Engine\System\Window.h>

namespace Engine
{
	KeyboardNotifier* KeyboardNotifier::_instance = nullptr;

	void _keyboardUpdate(uint8_t i_Key, bool i_down, uint16_t i_x, uint16_t i_y)
	{
		KeyboardNotifier::_instance->notifyKeyboardUpdate(i_Key, i_down, i_x, i_y);
	}

	void _mouseClickUpdate(uint8_t i_button, bool i_down, uint16_t i_x, uint16_t i_y)
	{
		KeyboardNotifier::_instance->notifyMouseClickUpdate(i_button, i_down, i_x, i_y);
	}

	void _mouseMoveUpdate(bool i_leftBt, bool i_rightBt, bool i_middleBt, uint16_t i_x, uint16_t i_y)
	{
		KeyboardNotifier::_instance->notifyMouseMoveUpdate(i_leftBt, i_rightBt, i_middleBt, i_x, i_y);
	}

	void _mousePassiveMoveUpdate(uint16_t i_x, uint16_t i_y)
	{
		KeyboardNotifier::_instance->notifyMousePassiveMoveUpdate(i_x, i_y);
	}

	void _mouseWheelUpdate(bool i_direction, uint16_t i_x, uint16_t i_y)
	{
		KeyboardNotifier::_instance->notifyMouseWheelUpdate(i_direction, i_x, i_y);
	}

	void KeyboardNotifier::notifyKeyboardUpdate(uint8_t i_Key, bool i_down, uint16_t i_x, uint16_t i_y)
	{
		std::vector<IKeyboardListener*>::iterator itr = _list.begin();

		for (; itr != _list.end(); itr++)
		{
			(*itr)->keyboardUpdate(i_Key, i_down, i_x, i_y);
		}
	}

	void KeyboardNotifier::notifyMouseClickUpdate(uint8_t i_button, bool i_down, uint16_t i_x, uint16_t i_y)
	{
		std::vector<IKeyboardListener*>::iterator itr = _list.begin();

		for (; itr != _list.end(); itr++)
		{
			(*itr)->mouseClickUpdate(i_button, i_down, i_x, i_y);
		}
	}

	void KeyboardNotifier::notifyMouseMoveUpdate(bool i_leftBt, bool i_rightBt, bool i_middleBt, uint16_t i_x, uint16_t i_y)
	{
		std::vector<IKeyboardListener*>::iterator itr = _list.begin();

		for (; itr != _list.end(); itr++)
		{
			(*itr)->mouseMoveUpdate(i_leftBt, i_rightBt, i_middleBt, i_x, i_y);
		}
	}

	void KeyboardNotifier::notifyMousePassiveMoveUpdate(uint16_t i_x, uint16_t i_y)
	{
		std::vector<IKeyboardListener*>::iterator itr = _list.begin();

		for (; itr != _list.end(); itr++)
		{
			(*itr)->mousePassiveMoveUpdate(i_x, i_y);
		}
	}

	void KeyboardNotifier::notifyMouseWheelUpdate(bool i_direction, uint16_t i_x, uint16_t i_y)
	{
		std::vector<IKeyboardListener*>::iterator itr = _list.begin();

		for (; itr != _list.end(); itr++)
		{
			(*itr)->mouseWheelUpdate(i_direction, i_x, i_y);
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
		SetMouseClickStateChangeCallback(&_mouseClickUpdate);
		SetMouseMoveStateChangeCallback(&_mouseMoveUpdate);
		SetMousePassiveMoveStateChangeCallback(&_mousePassiveMoveUpdate);
		SetMouseWheelMoveStateChangeCallback(&_mouseWheelUpdate);
	}

	KeyboardNotifier::~KeyboardNotifier()
	{
		_list.clear();
	}
}