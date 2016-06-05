#include <Engine\Util\Assert.h>
#include <Engine\Util\ConsolePrint.h>

#include <GraphicsDX\GraphicsDX.h>
#include <GraphicsDX\System\Device.h>
#include <GraphicsDX\System\DXHelpers.h>
#include <GraphicsDX\System\Window.h>
#include <GraphicsDX\Buffers\DrawResourcesBuffer.h>
#include <GraphicsDX\Renderer\RendererManager.h>

#include <Windows.h>

namespace GraphicsDX
{
	void Initialize( HINSTANCE i_hInstance, int i_CmdShow, const char * i_pWindowName, int i_IconID, unsigned int i_WindowWidth, unsigned int i_WindowHeight )
	{
		g_hInstance = i_hInstance;

#ifdef  UNICODE
		const size_t		lenWindowName = 128;
		wchar_t				WindowName[lenWindowName];
		
		MultiByteToWideChar( CP_ACP, 0, ( i_pWindowName ) ? i_pWindowName : "GraphicsDirectX Render", -1, WindowName, lenWindowName );

		LPCTSTR pWindowName = WindowName;
#else
		LPCTSTR pWindowName = ( i_pWindowName ) ? i_pWindowName : "GraphicsDirectX Render";
#endif // UNICODE

		g_hWnd = _CreateWindow( WndProc, g_hInstance, i_IconID, pWindowName, i_WindowWidth, i_WindowHeight );
		if( g_hWnd == nullptr )
		{
			DEBUG_PRINT( "_CreateWindow() failed.\n" );
			return;
		}

		HRESULT hResult = CreateDevice( g_hWnd );
		if( !Succeeded( hResult ) )
		{
			DEBUG_PRINT( "_CreateDevice() failed. HRESULT = %d.\n", hResult );
			return;
		}

		ShowWindow( g_hWnd, i_CmdShow );

		RendererManager::Initialize();
	}

	void Shutdown()
	{
		RendererManager::Shutdown();
		ReleaseDevice();
	}
} // namespace GraphicsDX