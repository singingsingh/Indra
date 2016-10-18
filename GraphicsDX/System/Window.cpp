#include <Engine\Util\Assert.h>
#include <Engine\Util\ConsolePrint.h>

#include <Windows.h>

namespace GraphicsDX
{
	HINSTANCE				g_hInstance = nullptr;
	HWND					g_hWnd = nullptr;
	const bool FULL_SCREEN = false;
	int screenWidth, screenHeight;

	LRESULT CALLBACK WndProc( HWND i_hWnd, UINT i_Message, WPARAM i_wParam, LPARAM i_lParam )
	{
		switch( i_Message )
		{
		case WM_PAINT:
		{
			PAINTSTRUCT			ps;
			
			HDC hdc = BeginPaint( i_hWnd, &ps );
			EndPaint( i_hWnd, &ps );
			break;
		}
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		default:
			return DefWindowProc( i_hWnd, i_Message, i_wParam, i_lParam );
		}

		return 0;
	}

	HWND _CreateWindow( WNDPROC i_pWndProc, HINSTANCE i_hInstance, INT i_IconID, LPCTSTR i_pWindowName, unsigned int i_Width, unsigned int i_Height )
	{
		//const LPCWSTR		pWindowClass = L"GLibWindowClass";

		//if( i_hInstance == NULL )
		//	i_hInstance = reinterpret_cast<HINSTANCE>( GetModuleHandle( NULL ) );
		//Assert( i_hInstance != NULL );

		//WNDCLASSEX			wcex;
		//wcex.cbSize = sizeof( WNDCLASSEX );
		//wcex.style = CS_HREDRAW | CS_VREDRAW;
		//wcex.lpfnWndProc = i_pWndProc;
		//wcex.cbClsExtra = 0;
		//wcex.cbWndExtra = 0;
		//wcex.hInstance = i_hInstance;
		//wcex.hIcon = i_IconID >= 0 ? LoadIcon( i_hInstance, LPCTSTR( i_IconID ) ) : nullptr;
		//wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
		//wcex.hbrBackground = (HBRUSH) ( COLOR_WINDOW + 1 );
		//wcex.lpszMenuName = nullptr;
		//wcex.lpszClassName = pWindowClass;
		//wcex.hIconSm = i_IconID >= 0 ? LoadIcon( i_hInstance, LPCTSTR( i_IconID ) ) : nullptr;
		//if( !RegisterClassEx( &wcex ) )
		//{
		//	DEBUG_PRINT( "Failure calling RegisterClassEx.\n" );
		//	return NULL;
		//}

		//// Create window
		//RECT			rc = { 0, 0, LONG( i_Width ), LONG( i_Height ) };

		//BOOL bResult = AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
		//Assert( bResult );

		//HWND hWindow = CreateWindow( pWindowClass, i_pWindowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		//				rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, i_hInstance, nullptr );

		//return hWindow;

		WNDCLASSEX wc;
		DEVMODE dmScreenSettings;
		int posX, posY;

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
		wc.lpszClassName = i_pWindowName;
		wc.cbSize = sizeof(WNDCLASSEX);

		// Register the window class.
		RegisterClassEx(&wc);

		// Determine the resolution of the clients desktop screen.
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
		if (FULL_SCREEN)
		{
			// If full screen set the screen to maximum size of the users desktop and 32bit.
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
			dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
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
			screenWidth = i_Width;
			screenHeight = i_Height;

			// Place the window in the middle of the screen.
			posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
			posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
		}

		// Create the window with the screen settings and get the handle to it.
		g_hWnd = CreateWindowEx(WS_EX_APPWINDOW, i_pWindowName, i_pWindowName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, screenWidth, screenHeight, NULL, NULL, i_hInstance, NULL);

		// Bring the window up on the screen and set it as main focus.
		ShowWindow(g_hWnd, SW_SHOW);
		SetForegroundWindow(g_hWnd);
		SetFocus(g_hWnd);

		// Hide the mouse cursor.
		ShowCursor(false);

		return;
	}
} // namespace GraphicsDX