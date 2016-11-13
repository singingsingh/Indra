#include <Engine\Graphics\WaterModel.h>

#include <External\Assimp\include\Importer.hpp>
#include <External\Assimp\include\postprocess.h>
#include <External\Assimp\include\scene.h>

#include <Engine\Util\ConsolePrint.h>
#include <Engine\Graphics\GraphicsDX.h>

#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		WaterModel::WaterModel()
		{
			_vertexBuffer = nullptr;
			_indexBuffer = nullptr;
			_gridWidth = 1.0f;
			_gridHeight = 1.0f;
			_gridRows = 5;
			_gridCols = 4;
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

			return true;
		}

		void WaterModel::shutdown()
		{
			shutdownBuffers();
			releaseModel();
		}

		void WaterModel::render()
		{
			renderBuffers();
		}

		int WaterModel::getIndexCount()
		{
			return _indexCount;
		}

		bool WaterModel::initializeBuffers()
		{
			VertexType* vertices;
			unsigned long* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;

			_vertexCount = (_gridCols+1)*(_gridRows+1);
			_indexCount = (_gridCols*_gridRows)* 2* 3;

			D3DXVECTOR3 corner(0.0f, 0.0f, 0.0f);

			float halfWidth = _gridCols*_gridWidth/2.0f;
			float halfHeight = _gridRows*_gridHeight/2.0f;
			corner.x = -halfWidth;
			corner.y = halfHeight;

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
					vertices[vertexCount].position = D3DXVECTOR3(col*_gridWidth, -row*_gridHeight, 0.0f) + corner;
					vertices[vertexCount].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
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

			// Set up the description of the static vertex buffer.
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

			ID3D11Device* device = GraphicsDX::GetDevice();
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

		void WaterModel::shutdownBuffers()
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

			return;
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

		void WaterModel::releaseModel()
		{
		}
	}
}