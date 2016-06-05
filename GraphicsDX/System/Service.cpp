#include <Engine\Util\Assert.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\System\Keyboard.h>

#include <GraphicsDX\System\Window.h>

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

namespace GraphicsDX
{
	void Service( bool & o_bQuitRequested )
	{
		o_bQuitRequested = false;

		BOOL				bGotMsg = FALSE;

		do
		{
			MSG					msg;
			
			msg.message = WM_NULL;
			
			bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

			if( bGotMsg )
			{
				if( TranslateAccelerator( g_hWnd, NULL, &msg ) == 0 )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}

				switch( msg.message )
				{
					case WM_QUIT:
					{
						o_bQuitRequested = true;
						break;
					}
					case WM_INPUT:
					{
						Engine::Keyboard::Service(HRAWINPUT(msg.lParam));
						break;
					}
					default:
					{
						//DEBUG_PRINT("message: 0x%04x\n", msg.message);
					}
				}
			}
		} while( ( bGotMsg == TRUE ) && ( o_bQuitRequested == false ) );

	}
} // namespace GraphicsDX