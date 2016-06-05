#ifndef __I_KEYBOARD_LISTENER_H__
#define __I_KEYBOARD_LISTENER_H__

namespace Engine
{
	class IKeyboardListener
	{
		public :
			virtual void keyboardUpdate(unsigned int i_VKeyID, bool i_bDown) = 0;
	};
}

#endif	//__I_KEYBOARD_LISTENER_H__
