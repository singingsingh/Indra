#include <Engine\Graphics\Model.h>

namespace Engine
{
	namespace Graphics
	{
		Model::Model()
		{
			_vertexBuffer = 0;
			_indexBuffer = 0;
		}

		Model::Model(const Model &other)
		{
		}

		Model::~Model()
		{
		}

		bool Model::initialize(ID3D11Device* i_device)
		{
			bool result;


			// Initialize the vertex and index buffer that hold the geometry for the triangle.
			result = initializeBuffers(i_device);
			if (!result)
			{
				return false;
			}

			return true;
		}

		void Model::shutdown()
		{
			shutdownBuffers();
		}

		void Model::render(ID3D11DeviceContext* i_deviceContext)
		{
			renderBuffers(i_deviceContext);
		}

		int Model::getIndexCount()
		{
			return _indexCount;
		}

		bool Model::initializeBuffers(ID3D11Device* i_device)
		{
			VertexType* vertices;
			unsigned long* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;

			// Set the number of vertices in the vertex array.
			_vertexCount = 4;

			// Set the number of indices in the index array.
			_indexCount = 4;

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
			vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
			vertices[0].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

			vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
			vertices[1].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

			vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
			vertices[2].color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

			vertices[3].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
			vertices[3].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;
			indices[3] = 3;

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

			// Now create the vertex buffer.
			result = i_device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
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
			result = i_device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
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

		void Model::shutdownBuffers()
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

		void Model::renderBuffers(ID3D11DeviceContext* i_deviceContext)
		{
			unsigned int stride;
			unsigned int offset;


			// Set vertex buffer stride and offset.
			stride = sizeof(VertexType);
			offset = 0;

			i_deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

			// Set the index buffer to active in the input assembler so it can be rendered.
			i_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
			//i_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			// Set the type of primitive that should be rendered from this vertex buffer.
			i_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		}
	}	// namespace Graphics
}	// namespace Engine