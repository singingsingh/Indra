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
			_gridWidth = 1;
			_gridHeight = 1;
			_gridRows = 4;
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

			_vertexCount = 4;
			_indexCount = 6;

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

			// Load the vertex array with data.
			vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
			vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);  // Top middle.
			vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
			vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			vertices[3].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);  // Bottom right.
			vertices[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// Load the index array with data.
			indices[0] = 0;  // Bottom left.
			indices[1] = 1;  // Top middle.
			indices[2] = 2;  // Bottom right.
			indices[3] = 3;  // Bottom right.
			indices[4] = 2;  // Bottom right.
			indices[5] = 1;  // Bottom right.

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