#include <Engine\Graphics\DebugWindow.h>

#include <Engine\System\Window.h>
#include <Engine\Graphics\Graphics.h>
#include <Engine\Util\Assert.h>

namespace Engine
{
	namespace Graphics
	{
		DebugWindow::DebugWindow(int i_width, int i_heigth)
		{
			_vertexBuffer = nullptr;
			_indexBuffer = nullptr;

			_screenWidth = System::Window::GetWidth();
			_screenHeight = System::Window::GetHeight();

			_bitmapWidth = i_width;
			_bitmapHeight = i_heigth;

			_previousPosX = -1;
			_previousPosY = -1;

			initializeBuffers();
		}

		DebugWindow::~DebugWindow()
		{
			_indexBuffer->Release();
			_indexBuffer = nullptr;

			_vertexBuffer->Release();
			_vertexBuffer = nullptr;
		}

		bool DebugWindow::render(int i_posX, int i_posY)
		{
			updateBuffers(i_posX, i_posY);
			renderBuffers();
			return true;
		}

		int DebugWindow::getIndexCount()
		{
			return _indexCount;
		}

		void DebugWindow::initializeBuffers()
		{
			VertexType* vertices;
			unsigned long* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;

			_vertexCount = 6;
			_indexCount = _vertexCount;

			vertices = new VertexType[_vertexCount];
			indices = new unsigned long[_indexCount];

			memset(vertices, 0, (sizeof(VertexType) * _vertexCount));

			for (int i = 0; i<_indexCount; i++)
			{
				indices[i] = i;
			}

			// create vertex buffer
			{
				vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
				vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
				vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				vertexBufferDesc.MiscFlags = 0;
				vertexBufferDesc.StructureByteStride = 0;

				vertexData.pSysMem = vertices;
				vertexData.SysMemPitch = 0;
				vertexData.SysMemSlicePitch = 0;

				result = GraphicsDX::GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
				MessagedAssert(SUCCEEDED(result), "Cannot create vertex buffer.");
			}

			// create index buffer
			{
				indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
				indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				indexBufferDesc.CPUAccessFlags = 0;
				indexBufferDesc.MiscFlags = 0;
				indexBufferDesc.StructureByteStride = 0;

				indexData.pSysMem = indices;
				indexData.SysMemPitch = 0;
				indexData.SysMemSlicePitch = 0;

				result = GraphicsDX::GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
				MessagedAssert(SUCCEEDED(result), "Cannot create index buffer.");
			}

			delete[] vertices;
			vertices = 0;

			delete[] indices;
			indices = 0;
		}


		void DebugWindow::updateBuffers(int positionX, int positionY)
		{
			float left, right, top, bottom;
			VertexType* vertices;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			VertexType* verticesPtr;
			HRESULT result;


			// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
			// currently has the correct parameters.
			if ((positionX == _previousPosX) && (positionY == _previousPosY))
			{
				return;
			}

			// If it has changed then update the position it is being rendered to.
			_previousPosX = positionX;
			_previousPosY = positionY;

			// Calculate the screen coordinates of the left side of the bitmap.
			left = (float)((_screenWidth / 2) * -1) + (float)positionX;

			// Calculate the screen coordinates of the right side of the bitmap.
			right = left + (float)_bitmapWidth;

			// Calculate the screen coordinates of the top of the bitmap.
			top = (float)(_screenHeight / 2) - (float)positionY;

			// Calculate the screen coordinates of the bottom of the bitmap.
			bottom = top - (float)_bitmapHeight;

			// Create the vertex array.
			vertices = new VertexType[_vertexCount];
			if (!vertices)
			{
				MessagedAssert(false, "Not able to allocate memory");
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
			result = GraphicsDX::GetDeviceContext()->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			MessagedAssert(SUCCEEDED(result), "Cannot map into vertex buffer.");

			verticesPtr = (VertexType*)mappedResource.pData;
			memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * _vertexCount));
			GraphicsDX::GetDeviceContext()->Unmap(_vertexBuffer, 0);

			delete[] vertices;
			vertices = nullptr;
		}

		void DebugWindow::renderBuffers()
		{
			unsigned int stride;
			unsigned int offset;

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			stride = sizeof(VertexType);
			offset = 0;

			deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
	}	// namespace Graphics
}	// namespace Engine