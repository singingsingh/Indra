#include <Engine\Util\Assert.h>

#include <GraphicsDX\System\Device.h>

#include <d3d11.h>
#include <DirectXColors.h>

namespace GraphicsDX
{
	bool BeginRendering( void )
	{
		Assert( g_pImmediateContext );
		Assert( g_pSwapChain );

		// Just clear the backbuffer
		g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, DirectX::Colors::MidnightBlue );

		if( g_pDepthStencilView )
			g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

		return true;
	}

	void EndRendering( void )
	{		
		HRESULT hResult = g_pSwapChain->Present( 0, 0 );
		Assert( hResult == S_OK );
	}

} // namespace GraphicsDX