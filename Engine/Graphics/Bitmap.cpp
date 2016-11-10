#include <Engine\Graphics\Bitmap.h>
#include <Engine\Graphics\GraphicsDX.h>

namespace Engine
{
	namespace Graphics
	{
		Bitmap::Bitmap()
		{
			_vertexBuffer = nullptr;
			_indexBuffer = nullptr;
			_texture = nullptr;
		}

		Bitmap::~Bitmap()
		{
		}

		bool Bitmap::initialize(int i_screenWidth, int i_screenHeight, const char* i_textureFilename, int i_bitmapWidth, int i_bitmapHeight)
		{
			bool result;

			// Store the screen size.
			_screenWidth = i_screenWidth;
			_screenHeight = i_screenHeight;

			// Store the size in pixels that this bitmap should be rendered at.
			_bitmapWidth = i_bitmapWidth;
			_bitmapHeight = i_bitmapHeight;

			// Initialize the previous rendering position to negative one.
			_previousPosX = -1;
			_previousPosY = -1;

			// Initialize the vertex and index buffers.
			result = initializeBuffers();
			if (!result)
			{
				return false;
			}

			// Load the texture for this model.
			result = loadTexture(i_textureFilename);
			if (!result)
			{
				return false;
			}

			return true;
		}

		void Bitmap::shutdown()
		{
			releaseTexture();
			shutdownBuffers();
		}

		bool Bitmap::render(int i_positionX, int i_positionY)
		{
			bool result;

			// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
			result = updateBuffers(i_positionX, i_positionY);
			if (!result)
			{
				return false;
			}

			// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
			renderBuffers();

			return true;
		}

		int Bitmap::getIndexCount()
		{
			return _indexCount;
		}

		ID3D11ShaderResourceView * Bitmap::getTexture()
		{
			return _texture->getTexture();
		}

		bool Bitmap::initializeBuffers()
		{
			VertexType* vertices;
			unsigned long* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;
			int i;

			// Set the number of vertices in the vertex array.
			_vertexCount = 6;

			// Set the number of indices in the index array.
			_indexCount = _vertexCount;

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

			// Initialize vertex array to zeros at first.
			memset(vertices, 0, (sizeof(VertexType) * _vertexCount));

			// Load the index array with data.
			for (i = 0; i<_indexCount; i++)
			{
				indices[i] = i;
			}

			// Set up the description of the static vertex buffer.
			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the vertex data.
			vertexData.pSysMem = vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			// Now create the vertex buffer.
			ID3D11Device* device = GraphicsDX::GetDevice();
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
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

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

		void Bitmap::shutdownBuffers()
		{
			_indexBuffer->Release();
			_indexBuffer = nullptr;

			_vertexBuffer->Release();
			_vertexBuffer = nullptr;
		}

		bool Bitmap::updateBuffers(int i_positionX, int i_positionY)
		{
			float left, right, top, bottom;
			VertexType* vertices;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			VertexType* verticesPtr;
			HRESULT result;

			// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
			// currently has the correct parameters.
			if ((i_positionX == _previousPosX) && (i_positionY == _previousPosY))
			{
				return true;
			}

			// If it has changed then update the position it is being rendered to.
			_previousPosX = i_positionX;
			_previousPosY = i_positionY;

			// Calculate the screen coordinates of the left side of the bitmap.
			left = (float)((_screenWidth / 2) * -1) + (float)i_positionX;

			// Calculate the screen coordinates of the right side of the bitmap.
			right = left + (float)_bitmapWidth;

			// Calculate the screen coordinates of the top of the bitmap.
			top = (float)(_screenHeight / 2) - (float)i_positionY;

			// Calculate the screen coordinates of the bottom of the bitmap.
			bottom = top - (float)_bitmapHeight;

			// Create the vertex array.
			vertices = new VertexType[_vertexCount];
			if (!vertices)
			{
				return false;
			}

			// Load the vertex array with data.
			// First triangle.
			vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
			vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

			vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
			vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

			vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
			vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

			// Second triangle.
			vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
			vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

			vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
			vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

			vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
			vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

			// Lock the vertex buffer so it can be written to.
			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			result = deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			// Get a pointer to the data in the vertex buffer.
			verticesPtr = (VertexType*)mappedResource.pData;

			// Copy the data into the vertex buffer.
			memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * _vertexCount));

			// Unlock the vertex buffer.
			deviceContext->Unmap(_vertexBuffer, 0);

			// Release the vertex array as it is no longer needed.
			delete[] vertices;
			vertices = 0;

			return true;
		}

		void Bitmap::renderBuffers()
		{
			unsigned int stride;
			unsigned int offset;

			stride = sizeof(VertexType);
			offset = 0;

			ID3D11DeviceContext* i_deviceContext = GraphicsDX::GetDeviceContext();
			i_deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
			i_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			i_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		bool Bitmap::loadTexture(const char* i_textureFileName)
		{
			bool result;

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

		void Bitmap::releaseTexture()
		{
			_texture->shutdown();
			delete _texture;
			_texture = nullptr;
		}
	}
}