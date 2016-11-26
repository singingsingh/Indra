#include <Engine\Graphics\WaterModel.h>

#include <Engine\Util\ConsolePrint.h>
#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Graphics\RenderTexture.h>
#include <Engine\Graphics\BuildWave.h>
#include <Engine\Graphics\BuildWaveShader.h>
#include <Engine\System\Window.h>

#include <math.h>
#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		WaterModel::WaterModel()
			:_singleWaveRTT(new RenderTexture(256, 256)),
			_gridRows(64),
			_gridCols(64),
			_heightFieldRTT(new RenderTexture(1024, 1024))
		{
			D3DXVECTOR4 heightFieldClearColor(0.0, 0.0, 0.0, 1.0);
			_heightFieldRTT->setClearColor(heightFieldClearColor);

			_vertexBuffer = nullptr;
			_indexBuffer = nullptr;
			_gridGap = 0.3f;

			float halfWidth = _gridCols*_gridGap * 0.5f;
			float halfHeight = _gridRows*_gridGap * 0.5f;
			_corner.x = -halfWidth;
			_corner.y = halfHeight;

			buildWaveParticle();
		}

		WaterModel::~WaterModel()
		{
		}

		bool WaterModel::initialize()
		{
			bool result;

			// Initialize the vertex and index buffer that hold the geometry for the triangle.
			result = initializeBuffers();
			if (!result)
			{
				return false;
			}

			_waveParticlesRTTModel = new WaveParticlesRTTModel();
			if (!_waveParticlesRTTModel)
			{
				return false;
			}

			// Initialize the model object.
			result = _waveParticlesRTTModel->initialize();
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}

			// Create the texture shader object.
			_waveParticlesRTTShader = new WaveParticlesRTTShader;
			if (!_waveParticlesRTTShader)
			{
				return false;
			}

			// Initialize the texture shader object.
			result = _waveParticlesRTTShader->initialize();
			if (!result)
			{
				MessageBox(System::Window::GetWindwsHandle(), "Could not initialize the texture shader object.", "Error", MB_OK);
				return false;
			}

			D3D11_BLEND_DESC blendStateDescription;
			ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
			blendStateDescription.AlphaToCoverageEnable = FALSE;
			blendStateDescription.IndependentBlendEnable = FALSE;
			blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
			blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			// Create the blend state using the description.
			ID3D11Device* device = GraphicsDX::GetDevice();
			HRESULT result1 = device->CreateBlendState(&blendStateDescription, &_additiveBlending);
			if (FAILED(result1))
			{
				return false;
			}

			D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
			ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

			depthDisabledStencilDesc.DepthEnable = false;
			depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
			depthDisabledStencilDesc.StencilEnable = false;
			depthDisabledStencilDesc.StencilReadMask = 0xFF;
			depthDisabledStencilDesc.StencilWriteMask = 0x0;
			depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			result1 = device->CreateDepthStencilState(&depthDisabledStencilDesc, &_disableDepthStencil);
			if (FAILED(result1))
			{
				return false;
			}

			return true;
		}

		void WaterModel::shutdown()
		{
			_waveParticlesRTTModel->shutdown();
			delete _waveParticlesRTTModel;
			_waveParticlesRTTModel = nullptr;

			_waveParticlesRTTShader->shutdown();
			delete _waveParticlesRTTShader;
			_waveParticlesRTTShader = nullptr;

			_indexBuffer->Release();
			_indexBuffer = 0;

			_vertexBuffer->Release();
			_vertexBuffer = 0;

			delete _singleWaveRTT;
			_singleWaveRTT = nullptr;

			delete _heightFieldRTT;
			_heightFieldRTT = nullptr;
		}

		void WaterModel::render()
		{
			updateHeightField();
			renderBuffers();
		}

		int WaterModel::getIndexCount()
		{
			return _indexCount;
		}

		ID3D11ShaderResourceView * WaterModel::getHeightField()
		{
			return _heightFieldRTT->getRenderTargetTexture();;
		}

		void WaterModel::spawnParticles()
		{
			_waveParticlesRTTModel->spawnParticles();
		}

		bool WaterModel::initializeBuffers()
		{
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;

			_vertexCount = (_gridCols + 1)*(_gridRows + 1);
			_indexCount = (_gridCols*_gridRows) * 2 * 3;

			VertexType* vertices;
			unsigned long* indices;

			vertices = new VertexType[_vertexCount];
			if (!vertices)
			{
				return false;
			}

			indices = new unsigned long[_indexCount];
			if (!indices)
			{
				return false;
			}

			int vertexCount = 0;
			
			for (uint8_t row = 0; row <= _gridRows; row++)
			{
				for (uint8_t col = 0; col <= _gridCols; col++)
				{
					vertices[vertexCount].pos = D3DXVECTOR2(col*_gridGap, -row*_gridGap) + _corner;
					vertices[vertexCount].tex = D3DXVECTOR2(float(col)/_gridCols, float(row) / _gridRows);
					vertexCount++;
				}
			}

			int indexCount = 0;
			for (uint8_t row = 0; row < _gridRows; row++)
			{
				for (uint8_t col = 0; col < _gridCols; col++)
				{
					indices[indexCount] = row * (_gridCols + 1) + col;
					indexCount++;

					indices[indexCount] = (row + 1) * (_gridCols + 1) + col + 1;
					indexCount++;

					indices[indexCount] = (row + 1) * (_gridCols + 1) + col;
					indexCount++;

					indices[indexCount] = row * (_gridCols + 1) + col;
					indexCount++;

					indices[indexCount] = row * (_gridCols + 1) + col + 1;
					indexCount++;

					indices[indexCount] = (row + 1) * (_gridCols + 1) + col + 1;
					indexCount++;
				}
			}

			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			vertexData.pSysMem = vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			ID3D11Device* device = GraphicsDX::GetDevice();
			result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			indexData.pSysMem = indices;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			delete[] vertices;
			vertices = nullptr;

			delete[] indices;
			indices = nullptr;

			return true;
		}

		void WaterModel::renderBuffers()
		{
			unsigned int stride;
			unsigned int offset;

			stride = sizeof(VertexType);
			offset = 0;

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		void WaterModel::updateHeightField()
		{
			float blendFactor[4];
			UINT blendSampleMask, stencilRefValue;

			_heightFieldRTT->beginRenderToTexture();

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			
			deviceContext->OMGetBlendState(&_prevBlendingState, blendFactor, &blendSampleMask);
			deviceContext->OMSetBlendState(_additiveBlending, nullptr, 0xffffffff);

			deviceContext->OMGetDepthStencilState(&_prevDepthStencil, &stencilRefValue);
			deviceContext->OMSetDepthStencilState(_disableDepthStencil, 1);
			
			_waveParticlesRTTModel->render();
			_waveParticlesRTTShader->render(_waveParticlesRTTModel->getVertexCount(), _singleWaveRTT->getRenderTargetTexture());

			deviceContext->OMSetBlendState(_prevBlendingState, blendFactor, blendSampleMask);
			deviceContext->OMSetDepthStencilState(_prevDepthStencil, stencilRefValue);

			_heightFieldRTT->endRenderToTexture();
		}

		void WaterModel::buildWaveParticle()
		{
			BuildWave* buildWaveModel = new BuildWave();
			BuildWaveShader* buildWaveShader = new BuildWaveShader();

			_singleWaveRTT->beginRenderToTexture();

			buildWaveModel->render();
			buildWaveShader->render(buildWaveModel->getIndexCount());

			_singleWaveRTT->endRenderToTexture();


			delete buildWaveModel;
			delete buildWaveShader;
		}
	}
}