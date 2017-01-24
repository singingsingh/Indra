#include <Engine\Graphics\AssimpMesh.h>

#include <External\Assimp\include\postprocess.h>

#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>
#include <Engine\Graphics\GraphicsDX.h>

#include <fstream>

namespace Engine
{
	namespace Graphics
	{
		AssimpMesh::AssimpMesh()
		{
			_meshArray = nullptr;
			_scene = nullptr;
			_defaultTexture = new Texture();
			_defaultTexture->initialize("Assets/Textures/SalesWatermark1.dds");
		}

		AssimpMesh::~AssimpMesh()
		{
			_defaultTexture->shutdown();
			delete _defaultTexture;
			_defaultTexture = nullptr;
		}

		void AssimpMesh::initialize(char* i_modelFilename)
		{
			loadModel(i_modelFilename);
			loadTexture();
		}

		void AssimpMesh::shutdown()
		{
			releaseTexture();
			shutdownBuffers();
		}

		void AssimpMesh::render(unsigned int meshIndex) const
		{
			unsigned int stride;
			unsigned int offset;

			stride = sizeof(VertexType);
			offset = 0;
			ID3D11DeviceContext* i_deviceContext = GraphicsDX::GetDeviceContext();
			i_deviceContext->IASetVertexBuffers(0, 1, &_meshArray[meshIndex].vertexBuffer, &stride, &offset);
			i_deviceContext->IASetIndexBuffer(_meshArray[meshIndex].indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			i_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		const aiScene * AssimpMesh::getScene() const
		{
			return _scene;
		}

		unsigned int AssimpMesh::getIndexCountOfMesh(unsigned int i_meshIndex) const
		{
			return _meshArray[i_meshIndex].indexCount;
		}

		ID3D11ShaderResourceView * AssimpMesh::getTexture(unsigned int i_meshIndex) const
		{
			if (_meshArray[i_meshIndex].texture == nullptr)
			{
				return _defaultTexture->getTexture();;
			}

			return _meshArray[i_meshIndex].texture->getTexture();
		}

		void AssimpMesh::shutdownBuffers()
		{
			for (unsigned int currentMesh = 0; currentMesh < _scene->mNumMeshes; currentMesh++)
			{
				_meshArray[currentMesh].indexBuffer->Release();
				_meshArray[currentMesh].indexBuffer = nullptr;

				_meshArray[currentMesh].vertexBuffer->Release();
				_meshArray[currentMesh].vertexBuffer = nullptr;

				_meshArray[currentMesh].texture = nullptr;
			}

			delete _meshArray;
		}

		void AssimpMesh::releaseTexture()
		{
			for (std::map<std::string, Texture*>::iterator itr = _textureIdMap.begin(); itr != _textureIdMap.end(); itr++)
			{
				itr->second->shutdown();
			}

			_textureIdMap.clear();
		}

		void AssimpMesh::loadModel(const char* i_modelFileName)
		{
			VertexType* vertices;
			unsigned long* indices;

			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;

			ID3D11Buffer *vertexBuffer, *indexBuffer;

			std::ifstream fileIn(i_modelFileName);
			MessagedAssert(!fileIn.fail(), "Cannot load the mesh file");

			_scene = importer.ReadFile(i_modelFileName, aiProcessPreset_TargetRealtime_Quality);
			MessagedAssert(_scene, "Error loading mesh file.");

			_meshArray = new Mesh[_scene->mNumMeshes];
			MessagedAssert(_meshArray, "Memory allocation failed.");

			for (unsigned int currentMesh = 0; currentMesh < _scene->mNumMeshes; currentMesh++)
			{
				const aiMesh* mesh = _scene->mMeshes[currentMesh];
				_meshArray[currentMesh].indexCount = _scene->mMeshes[currentMesh]->mNumFaces * 3;

				vertices = new VertexType[_scene->mMeshes[currentMesh]->mNumVertices];
				MessagedAssert(vertices, "Memory allocation failed.");

				indices = new unsigned long[_scene->mMeshes[currentMesh]->mNumFaces * 3];
				MessagedAssert(indices, "Memory allocation failed.");

				size_t index = 0;
				aiFace* face = nullptr;

				for (unsigned int currentFace = 0; currentFace < _scene->mMeshes[currentMesh]->mNumFaces; currentFace++)
				{
					face = &_scene->mMeshes[currentMesh]->mFaces[currentFace];
					indices[index] = face->mIndices[0];
					index++;
					indices[index] = face->mIndices[1];
					index++;
					indices[index] = face->mIndices[2];
					index++;
				}

				for (unsigned int currentVertex = 0; currentVertex < _scene->mMeshes[currentMesh]->mNumVertices; currentVertex++)
				{
					memcpy(&(vertices[currentVertex].position.x), &_scene->mMeshes[currentMesh]->mVertices[currentVertex], sizeof(float) * 3);
					memcpy(&(vertices[currentVertex].normal.x), &_scene->mMeshes[currentMesh]->mNormals[currentVertex], sizeof(float) * 3);

					if (!_scene->mMeshes[currentMesh]->HasTextureCoords(0))
					{
						vertices[currentVertex].texture.x = vertices[currentVertex].position.x;
						vertices[currentVertex].texture.y = vertices[currentVertex].position.y;
					}
					else
					{
						const aiVector3D* pTexCoord = &(_scene->mMeshes[currentMesh]->mTextureCoords[0][currentVertex]);
						vertices[currentVertex].texture.x = pTexCoord->x;
						vertices[currentVertex].texture.y = pTexCoord->y;
					}
				}

				vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				vertexBufferDesc.ByteWidth = sizeof(VertexType) * _scene->mMeshes[currentMesh]->mNumVertices;
				vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vertexBufferDesc.CPUAccessFlags = 0;
				vertexBufferDesc.MiscFlags = 0;
				vertexBufferDesc.StructureByteStride = 0;

				vertexData.pSysMem = vertices;
				vertexData.SysMemPitch = 0;
				vertexData.SysMemSlicePitch = 0;

				ID3D11Device* device = GraphicsDX::GetDevice();
				result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
				MessagedAssert(SUCCEEDED(result), "Failded to create vertex buffer.");
				_meshArray[currentMesh].vertexBuffer = vertexBuffer;

				indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				indexBufferDesc.ByteWidth = sizeof(unsigned long) * _scene->mMeshes[currentMesh]->mNumFaces * 3;
				indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				indexBufferDesc.CPUAccessFlags = 0;
				indexBufferDesc.MiscFlags = 0;
				indexBufferDesc.StructureByteStride = 0;

				indexData.pSysMem = indices;
				indexData.SysMemPitch = 0;
				indexData.SysMemSlicePitch = 0;

				result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
				MessagedAssert(SUCCEEDED(result), "Failed to create index buffer.");

				_meshArray[currentMesh].indexBuffer = indexBuffer;

				delete[] vertices;
				vertices = nullptr;

				delete[] indices;
				indices = nullptr;
			}
		}

		void AssimpMesh::loadTexture()
		{
			// collect all the textures
			for (unsigned int currentMaterial = 0; currentMaterial<_scene->mNumMaterials; currentMaterial++)
			{
				int texIndex = 0;
				aiString path;

				aiReturn texFound = _scene->mMaterials[currentMaterial]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
				while (texFound == AI_SUCCESS)
				{
					_textureIdMap[path.data] = 0;
					texIndex++;
					texFound = _scene->mMaterials[currentMaterial]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
				}
			}

			// create all the textures
			for (std::map<std::string, Texture*>::iterator itr = _textureIdMap.begin(); itr != _textureIdMap.end(); itr++)
			{
				itr->second = new Texture();
				std::string temp = "Assets/Textures/" + itr->first;
				itr->second->initialize(temp.c_str());
			}

			// Assign the texture to meshes
			
			for (unsigned int currentMesh = 0; currentMesh < _scene->mNumMeshes; currentMesh++)
			{
				const aiMesh* mesh = _scene->mMeshes[currentMesh];
				aiMaterial *mtl = _scene->mMaterials[mesh->mMaterialIndex];

				aiString texPath;
				if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath))
				{
					_meshArray[currentMesh].texture  = _textureIdMap[texPath.data];
				}
				else
				{
					_meshArray[currentMesh].texture = nullptr;
				}
			}
		}
	}
}
