#ifndef __RENDER_TEXTURE_H__
#define __RENDER_TEXTURE_H__

#include <d3d11.h>
#include <d3dx10math.h>

namespace Engine
{
	namespace Graphics
	{
		class RenderTexture
		{
		public:
			RenderTexture(int width, int height);
			~RenderTexture();

			void beginRenderToTexture();
			void endRenderToTexture();
			ID3D11ShaderResourceView* getRenderTargetTexture();
			ID3D11ShaderResourceView* getDepthTexture();
			ID3D11ShaderResourceView* getStencilTexture();
			void setClearColor(D3DXVECTOR4 clearColor);

		private:

			ID3D11Texture2D* _renderTargetTexture;
			ID3D11Texture2D* _depthStencilTexture;

			ID3D11RenderTargetView* _renderTargetView;
			ID3D11DepthStencilView* _depthStencilView;

			ID3D11RenderTargetView* _prevRenderTargetView;
			ID3D11DepthStencilView* _prevDepthStencilView;

			ID3D11ShaderResourceView* _renderTargetSRV;
			ID3D11ShaderResourceView* _depthSRV;
			ID3D11ShaderResourceView* _stencilSRV;

			D3DXVECTOR4 _clearColor;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__RENDER_TEXTURE_H__