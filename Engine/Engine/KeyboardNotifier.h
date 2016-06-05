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

			friend void _keyboardUpdate(unsigned int i_VKeyID, bool i_bDown);

		private:

			std::vector<IKeyboardListener*> _list;
			static KeyboardNotifier* _instance;

			KeyboardNotifier();
			~KeyboardNotifier();
			void notifyKeyboardUpdate(unsigned int i_VKeyID, bool i_bDown);
	};
}

#endif	//__KEYBOARD_NOTIFIER_H__
