#include <Engine\Graphics\SpecularModel.h>

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
		SpecularModel::SpecularModel()
		{
			_vertexBuffer = nullptr;
			_indexBuffer = nullptr;
			_texture = nullptr;
			_assimpData = nullptr;
		}

		SpecularModel::~SpecularModel()
		{
		}

		bool SpecularModel::initialize(char* i_modelFilename, const char* i_textureFilename)
		{
			bool result;

			// Load in the model data,
			result = loadModel(i_modelFilename);
			if (!result)
			{
				return false;
			}

			// Initialize the vertex and index buffer that hold the geometry for the triangle.
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

		void SpecularModel::shutdown()
		{
			releaseTexture();
			shutdownBuffers();
			releaseModel();
		}

		void SpecularModel::render()
		{
			renderBuffers();
		}

		int SpecularModel::getIndexCount()
		{
			return _indexCount;
		}

		ID3D11ShaderResourceView * SpecularModel::getTexture()
		{
			return _texture->getTexture();
		}

		bool SpecularModel::initializeBuffers()
		{
			VertexType* vertices;
			unsigned long* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;

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

			// Load the vertex array and index array with data.
			for (int i = 0; i < _vertexCount; i++)
			{
				vertices[i].position = D3DXVECTOR3(_assimpData[i].x, _assimpData[i].y, _assimpData[i].z);
				vertices[i].texture = D3DXVECTOR2(_assimpData[i].tu, _assimpData[i].tv);
				vertices[i].normal = D3DXVECTOR3(_assimpData[i].nx, _assimpData[i].ny, _assimpData[i].nz);

				//indices[i] = i;
			}

			for (int i = 0; i < _indexCount; i++)
			{
				indices[i] = _index[i];
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

		void SpecularModel::shutdownBuffers()
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

		void SpecularModel::renderBuffers()
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

		bool SpecularModel::loadModel(const char* i_modelFileName)
		{
			Assimp::Importer importer;
			const aiScene* scene = NULL;

			//check if file exists
			std::ifstream fileIn(i_modelFileName);
			if (!fileIn.fail())
			{
				fileIn.close();
			}
			else
			{
				DEBUG_PRINT("Couldn't open file: %s\n", i_modelFileName);
				DEBUG_PRINT("%s\n", importer.GetErrorString());
				return false;
			}

			scene = importer.ReadFile(i_modelFileName, aiProcessPreset_TargetRealtime_Quality);

			// If the import failed, report it
			if (!scene)
			{
				DEBUG_PRINT("%s\n", importer.GetErrorString());
				return false;
			}

			// Now we can access the file's contents.
			DEBUG_PRINT("Import of scene %s succeeded.", i_modelFileName);

			_vertexCount = scene->mMeshes[0]->mNumVertices;
			_indexCount = scene->mMeshes[0]->mNumFaces * 3;

			// Create the model using the vertex count that was read in.
			_assimpData = new AssimpData[_vertexCount];
			if (!_assimpData)
			{
				return false;
			}

			_index = new unsigned long[_indexCount];
			size_t index = 0;
			aiFace* face = nullptr;

			for (unsigned int currentFace = 0; currentFace < scene->mMeshes[0]->mNumFaces; currentFace++)
			{
				face = &scene->mMeshes[0]->mFaces[currentFace];
				_index[index] = face->mIndices[0];
				index++;
				_index[index] = face->mIndices[1];
				index++;
				_index[index] = face->mIndices[2];
				index++;
			}

			// Read in the vertex data.
			for (int i = 0; i < _vertexCount; i++)
			{
				memcpy(&_assimpData[i].x, &scene->mMeshes[0]->mVertices[i], sizeof(float) * 3);
				memcpy(&_assimpData[i].nx, &scene->mMeshes[0]->mNormals[i], sizeof(float) * 3);

				if (!scene->mMeshes[0]->HasTextureCoords(0))
				{
					_assimpData[i].tu = _assimpData[i].x;
					_assimpData[i].tv = _assimpData[i].y;
				}
				else
				{
					const aiVector3D* pTexCoord = &(scene->mMeshes[0]->mTextureCoords[0][i]);
					_assimpData[i].tu = pTexCoord->x;
					_assimpData[i].tv = pTexCoord->y;
				}
			}

			return true;
		}

		void SpecularModel::releaseModel()
		{
			delete[] _assimpData;
			_assimpData = nullptr;

			delete[] _index;
			_index = nullptr;

			return;
		}

		bool SpecularModel::loadTexture(const char* i_textureFileName)
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

		void SpecularModel::releaseTexture()
		{
			// Release the texture object.
			if (_texture)
			{
				_texture->shutdown();
				delete _texture;
				_texture = nullptr;
			}
		}

	}
}