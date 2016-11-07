#ifndef __KEYBOARD_NOTIFIER_H__
#define __KEYBOARD_NOTIFIER_H__

#include <Engine\Engine\IKeyboardListener.h>

#include <vector>

namespace Engine
{

	class KeyboardNotifier
	{
		public :

			static void CreateKeyboardNotifier();
			static void DestroyKeyboardNotifier();

			static void RegisterKeyboardUpdate(IKeyboardListener* keyboardListner);
			static void DeRegisterKeyboardUpdate(IKeyboardListener* keyboardListner);

			friend void _keyboardUpdate(uint8_t Key, bool down, uint16_t x, uint16_t y);
			friend void _mouseClickUpdate(uint8_t button, bool down, uint16_t x, uint16_t y);
			friend void _mouseMoveUpdate(bool leftBt, bool rightBt, bool middleBt, uint16_t x, uint16_t y);
			friend void _mousePassiveMoveUpdate(uint16_t x, uint16_t y);
			friend void _mouseWheelUpdate(bool direction, uint16_t x, uint16_t y);

		private:

			std::vector<IKeyboardListener*> _list;
			static KeyboardNotifier* _instance;

			KeyboardNotifier();
			~KeyboardNotifier();
			void notifyKeyboardUpdate(uint8_t Key, bool down, uint16_t x, uint16_t y);
			void notifyMouseClickUpdate(uint8_t button, bool down, uint16_t x, uint16_t y);
			void notifyMouseMoveUpdate(bool leftBt, bool rightBt, bool middleBt, uint16_t x, uint16_t y);
			void notifyMousePassiveMoveUpdate(uint16_t x, uint16_t y);
			void notifyMouseWheelUpdate(bool direction, uint16_t x, uint16_t y);
	};
}

#endif	//__KEYBOARD_NOTIFIER_H__
