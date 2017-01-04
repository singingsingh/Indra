#include <Engine\System\Window.h>

#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>

#include <Winuser.h>

namespace Engine
{
	KeyStateChange		_keyStateChangeCallback = nullptr;
	MouseClickStateChange		_mouseClickStateChangeCallback = nullptr;
	MouseMoveStateChange		_mouseMoveStateChangeCallback = nullptr;
	MousePassiveMoveStateChange		_mousePassiveMoveStateChangeCallback = nullptr;
	MouseWheelMoveStateChange		_mouseWheelMoveKeyStateChangeCallback = nullptr;

	void SetKeyStateChangeCallback(KeyStateChange i_callback)
	{
		_keyStateChangeCallback = i_callback;
	}

	void SetMouseClickStateChangeCallback(MouseClickStateChange i_callback)
	{
		_mouseClickStateChangeCallback = i_callback;
	}

	void SetMouseMoveStateChangeCallback(MouseMoveStateChange i_callback)
	{
		_mouseMoveStateChangeCallback = i_callback;
	}

	void SetMousePassiveMoveStateChangeCallback(MousePassiveMoveStateChange i_callback)
	{
		_mousePassiveMoveStateChangeCallback = i_callback;
	}

	void SetMouseWheelMoveStateChangeCallback(MouseWheelMoveStateChange i_callback)
	{
		_mouseWheelMoveKeyStateChangeCallback = i_callback;
	}


	bool KeyboardUpdate()
	{
		bool quit = false;
		BOOL bGotMsg = FALSE;

		do
		{
			MSG	msg;
			msg.message = WM_NULL;
			bGotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

			if (bGotMsg)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				switch (msg.message)
				{
					case WM_QUIT:
					{
						quit = true;
						break;
					}
				}
			}
		} while ((bGotMsg == TRUE) && (!quit));
		return quit;
	}

	namespace System
	{
		Window* Window::_instance = nullptr;

		Window::Window():
			FULL_SCREEN(false)
		{
		}

		bool Window::_initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight, const WORD* i_icon)
		{
			_requestedWidth = i_WindowWidth;
			_requestedHeight = i_WindowHeight;

			WNDCLASSEX wc;
			DEVMODE dmScreenSettings;

			_applicationName = i_pWindowName;
			_hinstance = i_hInstance;

			// Setup the windows class with default settings.
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.hInstance = i_hInstance;
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.lpfnWndProc = WndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			
			//wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
			wc.hIcon = LoadIcon(i_hInstance, MAKEINTRESOURCE(*i_icon));
			wc.hIconSm = wc.hIcon;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = i_pWindowName;
			

			// Register the window class.
			RegisterClassEx(&wc);

			_moniterWidth = GetSystemMetrics(SM_CXSCREEN);
			_moniterHeight = GetSystemMetrics(SM_CYSCREEN);

			// Determine the resolution of the clients desktop screen.
			_currentWidth = _moniterWidth;
			_currentHeight = _moniterHeight;

			// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
			if (FULL_SCREEN)
			{
				// If full screen set the screen to maximum size of the users desktop and 32bit.
				memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
				dmScreenSettings.dmSize = sizeof(dmScreenSettings);
				dmScreenSettings.dmPelsWidth = (unsigned long)_currentWidth;
				dmScreenSettings.dmPelsHeight = (unsigned long)_currentHeight;
				dmScreenSettings.dmBitsPerPel = 32;
				dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

				// Change the display settings to full screen.
				ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

				// Set the position of the window to the top left corner.
				_windowX = _windowY = 0;
			}
			else
			{
				// If windowed then set it to 800x600 resolution.
				_currentWidth = _requestedWidth;
				_currentHeight = _requestedHeight;

				// Place the window in the middle of the screen.
				_windowX = (_moniterWidth - _currentWidth) / 2;
				_windowY = (_moniterHeight - _currentHeight) / 2;
			}

			// Create the window with the screen settings and get the handle to it.
			_windowHandle = CreateWindowEx(WS_EX_APPWINDOW, i_pWindowName, i_pWindowName,
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
				_windowX, _windowY, _currentWidth, _currentHeight, NULL, NULL, i_hInstance, NULL);

			// Bring the window up on the screen and set it as main focus.
			ShowWindow(_windowHandle, SW_SHOW);
			SetForegroundWindow(_windowHandle);
			SetFocus(_windowHandle);

			// show the cursor
			ShowCursor(false);
			SetCursorPos(_moniterWidth/2, _moniterHeight/2);

			// clear all the keys to false
			memset(_keyStates, 0, _sizeReadTable);
			_mousePosX = 0;
			_mousePosY = 0;
			_leftButton = false;
			_rightButton = false;
			_middleButton = false;;

			return true;
		}

		bool Window::Destory()
		{
			MessagedAssert(_instance != nullptr, "Window instance is nullptr.");

			// Fix the display settings if leaving full screen mode.
			if (_instance->FULL_SCREEN)
			{
				ChangeDisplaySettings(NULL, 0);
			}

			// Remove the window.
			DestroyWindow(_instance->_windowHandle);
			_instance->_windowHandle = NULL;

			// Remove the application instance.
			UnregisterClass(_instance->_applicationName, _instance->_hinstance);
			_instance->_hinstance = NULL;

			delete _instance;
			return true;
		}

		HWND Window::GetWindwsHandle()
		{
			MessagedAssert(_instance, "Window not initialized.");
			return _instance->_windowHandle;
		}

		bool Window::IsFullScreen()
		{
			MessagedAssert(_instance, "Window not initialized.");
			return _instance->FULL_SCREEN;
		}

		int Window::GetWidth()
		{
			MessagedAssert(_instance, "Window not initialized.");
			return _instance->_currentWidth;
		}

		int Window::GetHeight()
		{
			MessagedAssert(_instance, "Window not initialized.");
			return _instance->_currentHeight;
		}

		LRESULT Window::messageHandler(HWND i_hWnd, UINT i_Message, WPARAM i_wParam, LPARAM i_lParam)
		{
			switch (i_Message)
			{
				case WM_SETFOCUS:
				{
					_active = true;
					DEBUG_PRINT("Window gained the focus.\n");
					break;
				}

				case WM_KILLFOCUS:
				{
					_active = false;
					DEBUG_PRINT("Window lost the focus.\n");
					break;
				}

				//case WM_ACTIVATE:
				//{
				//	if (LOWORD(i_wParam) == WA_ACTIVE)
				//	{
				//		DEBUG_PRINT("MEGAZORD ACTIVATED kew kew kew (flashy-eyes)\n");
				//	}
				//	else if (LOWORD(i_wParam) == WA_INACTIVE)
				//	{
				//		DEBUG_PRINT("I AM NOW INACTIVE.\n");
				//	}
				//	break;
				//}
				
				case WM_KEYDOWN:
				{
					if (!_keyStates[i_wParam])
					{
						_keyStates[i_wParam] = true;
					}
					_keyStateChangeCallback(static_cast<uint8_t>(i_wParam), true, _mousePosX, _mousePosY);
					break;
				}
				case WM_KEYUP:
				{
					if (_keyStates[i_wParam])
					{
						_keyStates[i_wParam] = false;
						_keyStateChangeCallback(static_cast<uint8_t>(i_wParam), false, _mousePosX, _mousePosY);
					}
					break;
				}
				case WM_MOUSEMOVE:
				{
					_mousePosX = LOWORD(i_lParam);
					_mousePosY = HIWORD(i_lParam);

					if (_leftButton || _rightButton || _middleButton)
					{
						_mouseMoveStateChangeCallback(_leftButton, _rightButton, _middleButton, _mousePosX, _mousePosY);
					}
					else
					{
						_mousePassiveMoveStateChangeCallback(_mousePosX, _mousePosY);
					}
					break;
				}
				case WM_LBUTTONDOWN:
				{
					if (!_leftButton)
					{
						_leftButton = true;
						_mouseClickStateChangeCallback(0, _leftButton, _mousePosX, _mousePosY);
					}
					break;
				}
				case WM_LBUTTONUP:
				{
					if (_leftButton)
					{
						_leftButton = false;
						_mouseClickStateChangeCallback(0, _leftButton, _mousePosX, _mousePosY);
					}
					break;
				}
				case WM_RBUTTONDOWN:
				{
					if (!_rightButton)
					{
						_rightButton = true;
						_mouseClickStateChangeCallback(1, _rightButton, _mousePosX, _mousePosY);
					}
					break;
				}
				case WM_RBUTTONUP:
				{
					if (_rightButton)
					{
						_rightButton = false;
						_mouseClickStateChangeCallback(1, _rightButton, _mousePosX, _mousePosY);
					}
					break;
				}
				case WM_MBUTTONDOWN:
				{
					if (!_middleButton)
					{
						_middleButton = true;
						_mouseClickStateChangeCallback(2, _middleButton, _mousePosX, _mousePosY);
					}
					break;
				}
				case WM_MBUTTONUP:
				{
					if (_middleButton)
					{
						_middleButton = false;
						_mouseClickStateChangeCallback(2, _middleButton, _mousePosX, _mousePosY);
					}
					break;
				}
				case WM_MOUSEWHEEL:
				{
					bool roll = false;
					short wheel_delta = GET_WHEEL_DELTA_WPARAM(i_wParam);
					if (wheel_delta > 0)
					{
						roll = true;
					}
					_mouseWheelMoveKeyStateChangeCallback(roll, _mousePosX, _mousePosY);
					break;
				}
			}
			return DefWindowProc(i_hWnd, i_Message, i_wParam, i_lParam);
		}

		void Window::SetCursor(uint16_t i_posX, uint16_t i_posY)
		{
			i_posX = int ((_instance->_moniterWidth - _instance->_currentWidth) * 0.5f + i_posX);
			i_posY = int ((_instance->_moniterHeight - _instance->_currentHeight) * 0.5f + i_posY);
			
			if (_instance->_mousePosX != i_posX || _instance->_mousePosY != i_posY)
			{
				SetCursorPos(i_posX, i_posY);
			}

			SetCursorPos(i_posX, i_posY);
		}

		void Window::SetCursorToCenter()
		{
			int newX = int(_instance->_moniterWidth * 0.5f);
			int newY = int(_instance->_moniterHeight * 0.5f);
			if (_instance->_mousePosX != uint16_t(_instance->_currentWidth*0.5f) || _instance->_mousePosY != uint16_t(_instance->_currentHeight*0.5f))
			{
				SetCursorPos(newX, newY);
			}
		}

		bool Window::IsActive()
		{
			return _instance->_active;
		}

		bool Window::Initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight, const WORD* i_icon)
		{
			if (_instance == nullptr)
			{
				_instance = new Window();
				DEBUG_PRINT("Creating Window singleton object\n");
				return _instance->_initialize(i_hInstance, i_pWindowName, i_WindowWidth, i_WindowHeight, i_icon);
			}
			else
			{
				MessagedAssert(false, "Cannot create second Graphics instance");
				DEBUG_PRINT("Cannot create second Graphics instance\n");
				return false;
			}
		}

		LRESULT CALLBACK Window::WndProc(HWND i_hWnd, UINT i_Message, WPARAM i_wParam, LPARAM i_lParam)
		{
			static bool postQuitMsg = false;
			
			if (!postQuitMsg&& _instance->_keyStates[VK_ESCAPE])
			{
				postQuitMsg = true;
				PostQuitMessage(0);
				return 0;
			}

			return _instance->messageHandler(i_hWnd, i_Message, i_wParam, i_lParam);
		}
	}	// System namespace
}	// Engine namespace