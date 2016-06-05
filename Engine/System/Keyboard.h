#pragma once

#include <Windows.h>

namespace Engine
{
	typedef void(*KeyStateChange)(unsigned int i_VKeyID, bool i_bDown);
	void SetKeyStateChangeCallback(KeyStateChange i_Callback);

	namespace Keyboard
	{
		bool Initialize();
		void Service(HRAWINPUT i_Input);
		void Shutdown();
	} // namespace Input

} // namespace GraphicsDX
