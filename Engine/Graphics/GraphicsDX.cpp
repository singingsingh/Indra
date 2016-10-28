#include <Engine\Graphics\GraphicsDX.h>

#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>

namespace Engine
{
	GraphicsDX* GraphicsDX::_instance = nullptr;

	GraphicsDX::GraphicsDX()
	{
		_swapChain = nullptr;
		_device = nullptr;
		_deviceContext = nullptr;
		_renderTargetView = nullptr;
		_depthStencilBuffer = nullptr;
		_depthStencilState = nullptr;
		_depthStencilView = nullptr;
		_rasterState = nullptr;
	}

	GraphicsDX::~GraphicsDX()
	{
	}

	bool GraphicsDX::_initialize(int i_screenWidth, int i_screenHeight, bool vsync, HWND hwnd, bool fullscreen)
	{
		HRESULT result;
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes, i, numerator, denominator, stringLength;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		int error;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Texture2D* backBufferPtr;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		D3D11_RASTERIZER_DESC rasterDesc;
		D3D11_VIEWPORT viewport;

		// Store the vsync setting.
		_vsyncEnabled = vsync;

		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
		{
			return false;
		}

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result))
		{
			return false;
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
		{
			return false;
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			return false;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			return false;
		}

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			return false;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		bool displayMode = false;
		for (i = 0; i<numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)i_screenWidth)
			{
				if (displayModeList[i].Height == (unsigned int)i_screenHeight)
				{
					displayMode = true;
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		if (!displayMode)
		{
			return false;
		}

		// Get the adapter (video card) description.
		result = adapter->GetDesc(&adapterDesc);
		if (FAILED(result))
		{
			return false;
		}

		// Store the dedicated video card memory in megabytes.
		_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
		error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			return false;
		}

		// Release the display mode list.
		delete[] displayModeList;
		displayModeList = nullptr;

		// Release the adapter output.
		adapterOutput->Release();
		adapterOutput = nullptr;

		// Release the adapter.
		adapter->Release();
		adapter = nullptr;

		// Release the factory.
		factory->Release();
		factory = nullptr;

		// Initialize the swap chain description.
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = i_screenWidth;
		swapChainDesc.BufferDesc.Height = i_screenHeight;

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if (_vsyncEnabled)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = hwnd;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		if (fullscreen)
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
		featureLevel = D3D_FEATURE_LEVEL_11_0;

		// Create the swap chain, Direct3D device, and Direct3D device context.
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, NULL, &_deviceContext);
		if (FAILED(result))
		{
			return false;
		}

		// Get the pointer to the back buffer.
		result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(result))
		{
			return false;
		}

		// Create the render target view with the back buffer pointer.
		result = _device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView);
		if (FAILED(result))
		{
			return false;
		}

		// Release pointer to the back buffer as we no longer need it.
		backBufferPtr->Release();
		backBufferPtr = 0;

		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = i_screenWidth;
		depthBufferDesc.Height = i_screenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
		if (FAILED(result))
		{
			return false;
		}

		// Set the depth stencil state.
		_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

		// Initailze the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
		if (FAILED(result))
		{
			return false;
		}

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
		if (FAILED(result))
		{
			return false;
		}

		// Now set the rasterizer state.
		_deviceContext->RSSetState(_rasterState);

		// Setup the viewport for rendering.
		viewport.Width = (float)i_screenWidth;
		viewport.Height = (float)i_screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		// Create the viewport.
		_deviceContext->RSSetViewports(1, &viewport);

		D3DXMatrixIdentity(&_worldMatrix);

		return true;
	}

	void GraphicsDX::Shutdown()
	{
		_instance->_shutdown();
		delete _instance;
		_instance = nullptr;
	}

	bool GraphicsDX::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen)
	{
		if (_instance == nullptr)
		{
			_instance = new GraphicsDX();
			DEBUG_PRINT("Creating GraphicsDX singleton object\n");
			return _instance->_initialize(screenWidth, screenHeight, vsync, hwnd, fullscreen);
		}
		else
		{
			MessagedAssert(false, "Cannot create second Graphics instance");
			DEBUG_PRINT("Cannot create second Graphics instance");
			return false;
		}
	}

	void GraphicsDX::_shutdown()
	{
		// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
		if (_swapChain)
		{
			_swapChain->SetFullscreenState(false, NULL);
		}

		if (_rasterState)
		{
			_rasterState->Release();
			_rasterState = 0;
		}

		if (_depthStencilView)
		{
			_depthStencilView->Release();
			_depthStencilView = 0;
		}

		if (_depthStencilState)
		{
			_depthStencilState->Release();
			_depthStencilState = 0;
		}

		if (_depthStencilBuffer)
		{
			_depthStencilBuffer->Release();
			_depthStencilBuffer = 0;
		}

		if (_renderTargetView)
		{
			_renderTargetView->Release();
			_renderTargetView = 0;
		}

		if (_deviceContext)
		{
			_deviceContext->Release();
			_deviceContext = 0;
		}

		if (_device)
		{
			_device->Release();
			_device = 0;
		}

		if (_swapChain)
		{
			_swapChain->Release();
			_swapChain = 0;
		}
	}

	void GraphicsDX::BeginScene(float red, float green, float blue, float alpha)
	{
		_instance->_beginScene(red, green, blue, alpha);
	}

	void GraphicsDX::EndScene()
	{
		_instance->_endScene();
	}

	void GraphicsDX::_beginScene(float red, float green, float blue, float alpha)
	{
		float color[4];


		// Setup the color to clear the buffer to.
		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;

		// Clear the back buffer.
		_deviceContext->ClearRenderTargetView(_renderTargetView, color);

		// Clear the depth buffer.
		_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		return;
	}

	void GraphicsDX::_endScene()
	{
		// Present the back buffer to the screen since rendering is complete.
		if (_vsyncEnabled)
		{
			// Lock to screen refresh rate.
			_swapChain->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			_swapChain->Present(0, 0);
		}

		return;
	}

	ID3D11Device * GraphicsDX::GetDevice()
	{
		return _instance->_device;
	}

	ID3D11DeviceContext * GraphicsDX::GetDeviceContext()
	{
		return _instance->_deviceContext;
	}

	D3DXMATRIX GraphicsDX::GetWorldMatrix()
	{
		return _instance->_worldMatrix;
	}

	void GraphicsDX::GetVideoCardInfo(char* cardName, int& memory)
	{
		strcpy_s(cardName, 128, _instance->_videoCardDescription);
		memory = _instance->_videoCardMemory;
	}
}