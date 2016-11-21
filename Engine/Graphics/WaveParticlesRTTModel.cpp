#include <Engine\Graphics\WaveParticlesRTTModel.h>

#include <Engine\Graphics\GraphicsDX.h>

namespace Engine
{
	namespace Graphics
	{

		WaveParticlesRTTModel::WaveParticlesRTTModel()
		{
			_vertexBuffer = nullptr;
			_texture = nullptr;
		}

		WaveParticlesRTTModel::~WaveParticlesRTTModel()
		{
		}

		bool WaveParticlesRTTModel::initialize(const char * i_textureFileName)
		{
			bool result;

			// Initialize the vertex and index buffer that hold the geometry for the triangle.
			result = initializeBuffers();
			if (!result)
			{
				return false;
			}

			// Load the texture for this model.
			result = loadTexture(i_textureFileName);
			if (!result)
			{
				return false;
			}

			return true;
		}

		void WaveParticlesRTTModel::shutdown()
		{
			//releaseTexture();
			shutdownBuffers();
		}

		void WaveParticlesRTTModel::render()
		{
			unsigned int stride;
			unsigned int offset;

			stride = sizeof(VertexType);
			offset = 0;

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		int WaveParticlesRTTModel::getVertexCount()
		{
			return _vertexCount;
		}

		ID3D11ShaderResourceView * WaveParticlesRTTModel::getTexture()
		{
			return _texture->getTexture();;
		}

		bool WaveParticlesRTTModel::initializeBuffers()
		{
			VertexType* vertices;
			D3D11_BUFFER_DESC vertexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData;
			HRESULT result;

			ID3D11Device* device = GraphicsDX::GetDevice();
			_vertexCount = 3;

			vertices = new VertexType[_vertexCount];
			if (!vertices)
			{
				return false;
			}

			vertices[0].position = D3DXVECTOR2(0.0f, 0.0f);
			vertices[0].texture = D3DXVECTOR2(0.0, 0.0);

			vertices[1].position = D3DXVECTOR2(0.0f, 100.0f);
			vertices[1].texture = D3DXVECTOR2(0.0, 1.0);

			vertices[2].position = D3DXVECTOR2(100.0f, 0.0f);
			vertices[2].texture = D3DXVECTOR2(1.0, 0.0);

			 // Set up the description of the vertex buffer.
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the vertex data.
			vertexData.pSysMem = vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			// Now create the vertex buffer.
			result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Release the arrays now that the vertex and index buffers have been created and loaded.
			delete[] vertices;
			vertices = 0;

			return true;
		}

		void WaveParticlesRTTModel::shutdownBuffers()
		{
			if (_vertexBuffer)
			{
				_vertexBuffer->Release();
				_vertexBuffer = 0;
			}
		}

		bool WaveParticlesRTTModel::loadTexture(const char * i_textureFileName)
		{
			bool result;

			// Create the texture object.
			_texture = new Texture;
			if (!_texture)
			{
				return false;
			}

			// Initialize the texture object.
			result = _texture->initialize(i_textureFileName);
			if (!result)
			{
				return false;
			}

			return true;
		}

		void WaveParticlesRTTModel::releaseTexture()
		{
			_texture->shutdown();
			delete _texture;
			_texture = nullptr;
		}
	}	// namespace Graphics
}	// namespace Engine
