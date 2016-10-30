#ifndef __WINDOWS_H__
#define	__WINDOWS_H__

#include <Windows.h>

namespace Engine
{
	namespace System
	{
		class Window
		{
		public:
			static bool Initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight);
			static bool Destory();
			static HWND GetWindwsHandle();
			static bool IsFullScreen();
			static int GetWidth();
			static int GetHeight();

		private:

			Window();

			bool _initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight);

			int _requestedWidth, _requestedHeight;
			int _currentWidth, _currentHeight;
			HWND _windowHandle;
			const bool FULL_SCREEN = false;

			static Window* _instance;
		};
	}
}

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#endif	//__WINDOWS_H__