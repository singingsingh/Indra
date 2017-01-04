#ifndef __WINDOWS_H__
#define	__WINDOWS_H__

#include <Windows.h>
#include <stdint.h>

namespace Engine
{
	typedef void(*KeyStateChange)(uint8_t key, bool down, uint16_t x, uint16_t y);
	typedef void(*MouseClickStateChange)(uint8_t button, bool down, uint16_t x, uint16_t y);
	typedef void(*MouseMoveStateChange)(bool leftBt, bool rightBt, bool middleBt, uint16_t x, uint16_t y);
	typedef void(*MousePassiveMoveStateChange)(uint16_t x, uint16_t y);
	typedef void(*MouseWheelMoveStateChange)(bool i_direction, uint16_t x, uint16_t y);

	void SetKeyStateChangeCallback(KeyStateChange callback);
	void SetMouseClickStateChangeCallback(MouseClickStateChange callback);
	void SetMouseMoveStateChangeCallback(MouseMoveStateChange callback);
	void SetMousePassiveMoveStateChangeCallback(MousePassiveMoveStateChange callback);
	void SetMouseWheelMoveStateChangeCallback(MouseWheelMoveStateChange callback);

	bool KeyboardUpdate();

	namespace System
	{
		class Window
		{
		public:
			static bool Initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight, const WORD* icon);
			static bool Destory();
			static HWND GetWindwsHandle();
			static bool IsFullScreen();
			static int GetWidth();
			static int GetHeight();
			LRESULT CALLBACK messageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			static void SetCursor(uint16_t posX, uint16_t posY);
			static void SetCursorToCenter();
			static bool IsActive();

		private:

			Window();

			bool _initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight, const WORD* icon);
			uint16_t _requestedWidth, _requestedHeight;
			uint16_t _currentWidth, _currentHeight;
			uint16_t _windowX, _windowY;
			uint16_t _moniterWidth, _moniterHeight;
			LPCSTR _applicationName;
			HINSTANCE _hinstance;
			HWND _windowHandle;
			const bool FULL_SCREEN;
			bool _active;

			const static size_t	_sizeReadTable = 256;
			bool  _keyStates[_sizeReadTable];
			bool _leftButton, _rightButton, _middleButton;
			uint16_t _mousePosX, _mousePosY;

			static Window* _instance;
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		};
	}
}



#endif	//__WINDOWS_H__