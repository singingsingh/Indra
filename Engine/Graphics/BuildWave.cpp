#include <Engine\Graphics\BuildWave.h>

#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Util\Assert.h>

namespace Engine
{
	namespace Graphics
	{
		BuildWave::BuildWave()
		{
			_vertexBuffer = nullptr;
			_indexBuffer = nullptr;

			VertexType* vertices;
			unsigned int* indices;

			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;

			ID3D11Device* device = GraphicsDX::GetDevice();
			uint8_t vertexCount = 4;

			_indexCount = 6;

			vertices = new VertexType[vertexCount];
			if (!vertices)
			{
				MessagedAssert(false, "Cannot allocate memory for Vertex data");
			}

			// Create the index array.
			indices = new unsigned int[_indexCount];
			if (!indices)
			{
				MessagedAssert(false, "Cannot allocate memory for Index data");
			}

			vertices[0].position = D3DXVECTOR2(-1.0f, -1.0f);
			vertices[1].position = D3DXVECTOR2(-1.0f, 1.0f);
			vertices[2].position = D3DXVECTOR2(1.0f, -1.0f);
			vertices[3].position = D3DXVECTOR2(1.0f, 1.0f);

			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;
			indices[3] = 1;
			indices[4] = 3;
			indices[5] = 2;

			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			vertexData.pSysMem = vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
			MessagedAssert(SUCCEEDED(result), "Cannot create vertex buffer.");

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
			MessagedAssert(SUCCEEDED(result), "Cannot create index buffer.");

			delete[] vertices;
			vertices = 0;

			delete[] indices;
			indices = 0;
		}

		BuildWave::~BuildWave()
		{
			_indexBuffer->Release();
			_indexBuffer = nullptr;

			_vertexBuffer->Release();
			_vertexBuffer = nullptr;
		}

		void BuildWave::render()
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

		uint8_t BuildWave::getIndexCount()
		{
			return _indexCount;
		}
	}	// namespace Graphics
}	// namespace Engine