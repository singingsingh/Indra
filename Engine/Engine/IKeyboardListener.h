#ifndef __I_KEYBOARD_LISTENER_H__
#define __I_KEYBOARD_LISTENER_H__

#include <stdint.h>

namespace Engine
{
	class IKeyboardListener
	{
		public :
			virtual void keyboardUpdate(uint8_t i_Key, bool i_down, uint16_t x, uint16_t y) = 0;
			virtual void mouseClickUpdate(uint8_t i_button, bool i_down, uint16_t x, uint16_t y) = 0;
			virtual void mouseMoveUpdate(bool leftBt, bool rightBt, bool middleBt, uint16_t x, uint16_t y) = 0;
			virtual void mousePassiveMoveUpdate(uint16_t x, uint16_t y) = 0;
			virtual void mouseWheelUpdate(bool i_direction, uint16_t x, uint16_t y) = 0;
	};
}

#endif	//__I_KEYBOARD_LISTENER_H__
