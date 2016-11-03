#ifndef __GRAPHICS_DX__
#define __GRAPHICS_DX__

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

namespace Engine
{
	class GraphicsDX
	{
	public:

		bool static Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen);

		void static Shutdown();

		void static BeginScene(float, float, float, float);
		void static EndScene();

		static ID3D11Device* GetDevice();
		static ID3D11DeviceContext* GetDeviceContext();

		static D3DXMATRIX GetWorldMatrix();
		static void GetVideoCardInfo(char*, int&);

		static void TurnZBufferOn();
		static void TurnZBufferOff();

		static void TurnOnAlphaBlending();
		static void TurnOffAlphaBlending();

	private:

		GraphicsDX();
		~GraphicsDX();

		bool _initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen);
		void _shutdown();
		void _beginScene(float, float, float, float);
		void _endScene();

		bool _vsyncEnabled;
		int _videoCardMemory;
		char _videoCardDescription[128];
		IDXGISwapChain* _swapChain;
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;
		ID3D11RenderTargetView* _renderTargetView;
		ID3D11Texture2D* _depthStencilBuffer;
		ID3D11DepthStencilState* _depthStencilState;
		ID3D11DepthStencilView* _depthStencilView;
		ID3D11RasterizerState* _rasterState;
		D3DXMATRIX _worldMatrix;
		ID3D11DepthStencilState* _depthDisabledStencilState;
		ID3D11BlendState* _alphaEnableBlendingState;
		ID3D11BlendState* _alphaDisableBlendingState;

		static GraphicsDX* _instance;
	};	// class GraphicsDX
}	// namespace Engine
#endif // __GRAPHICS_DX__