#include <Engine\Graphics\RenderTexture.h>

#include <Engine\Graphics\Graphics.h>
#include <Engine\Util\Assert.h>

namespace Engine
{
	namespace Graphics
	{

		RenderTexture::RenderTexture(int i_width, int i_height)
			:_clearColor(1.0f, 1.0f, 1.0f, 1.0f)
		{
			_renderTargetTexture = nullptr;
			_depthStencilTexture = nullptr;

			_renderTargetView = nullptr;
			_depthStencilView = nullptr;

			_renderTargetSRV = nullptr;
			_depthSRV = nullptr;
			_stencilSRV = nullptr;

			D3D11_TEXTURE2D_DESC renderTargetTextureDesc;
			D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
			HRESULT result;
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

			// Render Target View

			ZeroMemory(&renderTargetTextureDesc, sizeof(renderTargetTextureDesc));

			renderTargetTextureDesc.Width = i_width;
			renderTargetTextureDesc.Height = i_height;
			renderTargetTextureDesc.MipLevels = 1;
			renderTargetTextureDesc.ArraySize = 1;
			renderTargetTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			renderTargetTextureDesc.SampleDesc.Count = 1;
			renderTargetTextureDesc.Usage = D3D11_USAGE_DEFAULT;
			renderTargetTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			renderTargetTextureDesc.CPUAccessFlags = 0;
			renderTargetTextureDesc.MiscFlags = 0;

			result = GraphicsDX::GetDevice()->CreateTexture2D(&renderTargetTextureDesc, NULL, &_renderTargetTexture);
			if (FAILED(result))
			{
				MessagedAssert(false, "Creation of texture failed.");
			}

			renderTargetViewDesc.Format = renderTargetTextureDesc.Format;
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetViewDesc.Texture2D.MipSlice = 0;

			result = GraphicsDX::GetDevice()->CreateRenderTargetView(_renderTargetTexture, &renderTargetViewDesc, &_renderTargetView);
			if (FAILED(result))
			{
				MessagedAssert(false, "Creation of Render Target View failed.");
			}

			shaderResourceViewDesc.Format = renderTargetTextureDesc.Format;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			result = GraphicsDX::GetDevice()->CreateShaderResourceView(_renderTargetTexture, &shaderResourceViewDesc, &_renderTargetSRV);
			if (FAILED(result))
			{
				MessagedAssert(false, "Cannot create Shader Resource View.");
			}

			// Depth Stencil

			ZeroMemory(&depthStencilTextureDesc, sizeof(depthStencilTextureDesc));

			depthStencilTextureDesc.Width = i_width;
			depthStencilTextureDesc.Height = i_height;
			depthStencilTextureDesc.MipLevels = 1;
			depthStencilTextureDesc.ArraySize = 1;
			depthStencilTextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			depthStencilTextureDesc.SampleDesc.Count = 1;
			depthStencilTextureDesc.SampleDesc.Quality = 0;
			depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			depthStencilTextureDesc.CPUAccessFlags = 0;
			depthStencilTextureDesc.MiscFlags = 0;

			result = GraphicsDX::GetDevice()->CreateTexture2D(&depthStencilTextureDesc, NULL, &_depthStencilTexture);
			MessagedAssert(SUCCEEDED(result), "Failed to create depth texture.");

			ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;

			result = GraphicsDX::GetDevice()->CreateDepthStencilView(_depthStencilTexture, &depthStencilViewDesc, &_depthStencilView);
			MessagedAssert(SUCCEEDED(result), "Failed to create depth stencil view.");

			shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			result = GraphicsDX::GetDevice()->CreateShaderResourceView(_depthStencilTexture, &shaderResourceViewDesc, &_depthSRV);
			MessagedAssert(SUCCEEDED(result), "Failed to create Shader resource view");

			shaderResourceViewDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			result = GraphicsDX::GetDevice()->CreateShaderResourceView(_depthStencilTexture, &shaderResourceViewDesc, &_stencilSRV);
			MessagedAssert(SUCCEEDED(result), "Failed to create Shader resource view");

			_viewport.Width = (float)i_width;
			_viewport.Height = (float)i_height;
			_viewport.MinDepth = 0.0f;
			_viewport.MaxDepth = 1.0f;
			_viewport.TopLeftX = 0.0f;
			_viewport.TopLeftY = 0.0f;
		}

		RenderTexture::~RenderTexture()
		{
			_renderTargetTexture->Release();
			_renderTargetTexture = nullptr;

			_depthStencilTexture->Release();
			_depthStencilTexture = nullptr;

			_renderTargetView->Release();
			_renderTargetView = nullptr;

			_depthStencilView->Release();
			_depthStencilView = nullptr;

			_renderTargetSRV->Release();
			_renderTargetSRV = nullptr;

			_depthSRV->Release();
			_depthSRV = nullptr;

			_stencilSRV->Release();
			_stencilSRV = nullptr;
		}

		void RenderTexture::beginRenderToTexture()
		{
			GraphicsDX::GetDeviceContext()->OMGetRenderTargets(1, &_prevRenderTargetView, &_prevDepthStencilView);
			UINT numViewPort = 1;
			GraphicsDX::GetDeviceContext()->RSGetViewports(&numViewPort, &_prevViewport);

			GraphicsDX::GetDeviceContext()->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
			GraphicsDX::GetDeviceContext()->RSSetViewports(1, &_viewport);

			GraphicsDX::GetDeviceContext()->ClearRenderTargetView(_renderTargetView, _clearColor);
			GraphicsDX::GetDeviceContext()->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		void RenderTexture::endRenderToTexture()
		{
			GraphicsDX::GetDeviceContext()->OMSetRenderTargets(1, &_prevRenderTargetView, _prevDepthStencilView);
			GraphicsDX::GetDeviceContext()->RSSetViewports(1, &_prevViewport);
		}

		ID3D11ShaderResourceView * RenderTexture::getRenderTargetTexture()
		{
			return _renderTargetSRV;
		}

		ID3D11ShaderResourceView * RenderTexture::getDepthTexture()
		{
			return _depthSRV;
		}

		ID3D11ShaderResourceView * RenderTexture::getStencilTexture()
		{
			return _stencilSRV;
		}

		void RenderTexture::setClearColor(D3DXVECTOR4 i_clearColor)
		{
			_clearColor = i_clearColor;
		}
	}	// namespace Graphics
}	// namespace Engine