#include <Engine\System\Window.h>

#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>

namespace Engine
{
	namespace System
	{
		Window* Window::_instance = nullptr;

		Window::Window()
		{
		}

		bool Window::_initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight)
		{
			_requestedWidth = i_WindowWidth;
			_requestedHeight = i_WindowHeight;

			WNDCLASSEX wc;
			DEVMODE dmScreenSettings;
			int posX, posY;

			LPCSTR m_applicationName = i_pWindowName;

			// Setup the windows class with default settings.
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.lpfnWndProc = WndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = i_hInstance;
			wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
			wc.hIconSm = wc.hIcon;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = m_applicationName;
			wc.cbSize = sizeof(WNDCLASSEX);

			// Register the window class.
			RegisterClassEx(&wc);

			// Determine the resolution of the clients desktop screen.
			_currentWidth = GetSystemMetrics(SM_CXSCREEN);
			_currentHeight = GetSystemMetrics(SM_CYSCREEN);

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
				posX = posY = 0;
			}
			else
			{
				// If windowed then set it to 800x600 resolution.
				_currentWidth = _requestedWidth;
				_currentHeight = _requestedHeight;

				// Place the window in the middle of the screen.
				posX = (GetSystemMetrics(SM_CXSCREEN) - _currentWidth) / 2;
				posY = (GetSystemMetrics(SM_CYSCREEN) - _currentHeight) / 2;
			}

			// Create the window with the screen settings and get the handle to it.
			_windowHandle = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
				posX, posY, _currentWidth, _currentHeight, NULL, NULL, i_hInstance, NULL);

			// Bring the window up on the screen and set it as main focus.
			ShowWindow(_windowHandle, SW_SHOW);
			SetForegroundWindow(_windowHandle);
			SetFocus(_windowHandle);

			// Hide the mouse cursor.
			ShowCursor(false);

			return true;
		}

		bool Window::Destory()
		{
			MessagedAssert(_instance != nullptr, "Window instance is nullptr.");
			delete _instance;
			return true;
		}

		HWND Window::GetWindwsHangle()
		{
			return _instance->_windowHandle;
		}

		bool Window::IsFullScreen()
		{
			return _instance->FULL_SCREEN;
		}

		int Window::GetWidth()
		{
			return _instance->_currentWidth;
		}

		int Window::GetHeight()
		{
			return _instance->_currentHeight;
		}

		bool Window::Initialize(HINSTANCE i_hInstance, const char * i_pWindowName, unsigned int i_WindowWidth, unsigned int i_WindowHeight)
		{
			if (_instance == nullptr)
			{
				_instance = new Window();
				DEBUG_PRINT("Creating Window singleton object\n");
				return _instance->_initialize(i_hInstance, i_pWindowName, i_WindowWidth, i_WindowHeight);
			}
			else
			{
				MessagedAssert(false, "Cannot create second Graphics instance");
				DEBUG_PRINT("Cannot create second Graphics instance\n");
				return false;
			}
		}

	}	// System namespace
}	// Engine namespace

LRESULT CALLBACK WndProc(HWND i_hWnd, UINT i_Message, WPARAM i_wParam, LPARAM i_lParam)
{
	switch (i_Message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT			ps;

		HDC hdc = BeginPaint(i_hWnd, &ps);
		EndPaint(i_hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(i_hWnd, i_Message, i_wParam, i_lParam);
	}
	return 0;
}