#include <Engine\Graphics\TextureModel.h>

#include <Engine\Graphics\GraphicsDX.h>

namespace Engine
{
	namespace Graphics
	{

		TextureModel::TextureModel()
		{
			_vertexBuffer = 0;
			_indexBuffer = 0;
			_texture = 0;
		}

		TextureModel::~TextureModel()
		{
		}

		bool TextureModel::initialize(const char * i_textureFileName)
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

		void TextureModel::shutdown()
		{
			releaseTexture();
			shutdownBuffers();
		}

		void TextureModel::render()
		{
			renderBuffers();
		}

		int TextureModel::getIndexCount()
		{
			return _indexCount;
		}

		ID3D11ShaderResourceView * TextureModel::getTexture()
		{
			return _texture->getTexture();;
		}

		bool TextureModel::initializeBuffers()
		{
			VertexType* vertices;
			unsigned long* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;

			ID3D11Device* device = GraphicsDX::GetDevice();
			// Set the number of vertices in the vertex array.
			_vertexCount = 3;

			// Set the number of indices in the index array.
			_indexCount = 3;

			// Create the vertex array.
			vertices = new VertexType[_vertexCount];
			if (!vertices)
			{
				return false;
			}

			// Create the index array.
			indices = new unsigned long[_indexCount];
			if (!indices)
			{
				return false;
			}

			// Load the vertex array with data.
			vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
			vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

			vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
			vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);

			vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
			vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);

			// Load the index array with data.
			indices[0] = 0;  // Bottom left.
			indices[1] = 1;  // Top middle.
			indices[2] = 2;  // Bottom right.

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

			// Set up the description of the static index buffer.
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the index data.
			indexData.pSysMem = indices;

			// Create the index buffer.
			result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Release the arrays now that the vertex and index buffers have been created and loaded.
			delete[] vertices;
			vertices = 0;

			delete[] indices;
			indices = 0;

			return true;
		}

		void TextureModel::shutdownBuffers()
		{
			// Release the index buffer.
			if (_indexBuffer)
			{
				_indexBuffer->Release();
				_indexBuffer = 0;
			}

			// Release the vertex buffer.
			if (_vertexBuffer)
			{
				_vertexBuffer->Release();
				_vertexBuffer = 0;
			}
		}

		void TextureModel::renderBuffers()
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

		bool TextureModel::loadTexture(const char * i_textureFileName)
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

		void TextureModel::releaseTexture()
		{
			// Release the texture object.
			if (_texture)
			{
				_texture->shutdown();
				delete _texture;
				_texture = nullptr;
			}
		}
	}	// namespace Graphics
}	// namespace Engine
