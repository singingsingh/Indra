#pragma once

#include <Engine\System\Keyboard.h>

#include <Windows.h>

namespace GraphicsDX
{
	void Initialize( HINSTANCE i_hInstance, int i_CmdShow, const char * i_pWindowName, int i_IconID, unsigned int i_WindowWidth, unsigned int i_WindowHeight );
	void Shutdown();

	bool BeginRendering();
	void EndRendering();

	void Service( bool & o_bQuitRequested );
} // namespace GraphicsDX