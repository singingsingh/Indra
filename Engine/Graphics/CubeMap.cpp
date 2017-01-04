#include <Engine\Graphics\CubeMap.h>

#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Util\Assert.h>

namespace Engine
{
	namespace Graphics
	{
		CubeMap::CubeMap(const char * i_cubeMapPath)
		{
			_cubeMap = new Texture();
			if (!_cubeMap)
			{
				MessagedAssert(false, "Cannot allocate memory for cube map.");
			}

			_cubeMap->initialize(i_cubeMapPath);

			initializeBuffers();
		}

		CubeMap::~CubeMap()
		{
			_cubeMap->shutdown();
			delete _cubeMap;
			_cubeMap = nullptr;

			_vertexBuffer->Release();
			_vertexBuffer = nullptr;

			_indexBuffer->Release();
			_indexBuffer = nullptr;
		}

		void CubeMap::uploadVertexBuffers()
		{

			unsigned int stride;
			unsigned int offset;

			stride = sizeof(VertexType);
			offset = 0;

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		}

		uint8_t CubeMap::getIndexCount()
		{
			return _indexCount;
		}

		ID3D11ShaderResourceView* CubeMap::getTexture()
		{
			return _cubeMap->getTexture();
		}

		void CubeMap::initializeBuffers()
		{
			_vertexCount = 8;
			_indexCount = 36;

			VertexType* vertices = new VertexType[_vertexCount];
			if (!vertices)
			{
				MessagedAssert(false, "Cannot allocate memory for vertex data.");
			}

			unsigned long* indices = new unsigned long[_indexCount];
			if (!indices)
			{
				MessagedAssert(false, "Cannot allocate memory for index data.");
			}

			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;

			vertices[0].pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
			vertices[1].pos = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
			vertices[2].pos = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
			vertices[3].pos = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
			vertices[4].pos = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
			vertices[5].pos = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
			vertices[6].pos = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
			vertices[7].pos = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

			indices[0] = 4; indices[1] = 5; indices[2] = 6;
			indices[3] = 5; indices[4] = 7; indices[5] = 6;

			indices[6] = 6; indices[7] = 7; indices[8] = 2;
			indices[9] = 7; indices[10] = 3; indices[11] = 2;

			indices[12] = 2; indices[13] = 3; indices[14] = 0;
			indices[15] = 3; indices[16] = 1; indices[17] = 0;

			indices[18] = 0; indices[19] = 1; indices[20] = 4;
			indices[21] = 1; indices[22] = 5; indices[23] = 4;

			indices[24] = 5; indices[25] = 1; indices[26] = 7;
			indices[27] = 1; indices[28] = 3; indices[29] = 7;

			indices[30] = 0; indices[31] = 4; indices[32] = 2;
			indices[33] = 4; indices[34] = 6; indices[35] = 2;

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
			delete[] indices;
		}
	}
}