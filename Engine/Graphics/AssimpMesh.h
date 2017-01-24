#ifndef __ASSIMP_MODEL_H__
#define __ASSIMP_MODEL_H__

#include <Engine\Graphics\Texture.h>

#include <External\Assimp\include\Importer.hpp>
#include <External\Assimp\include\scene.h>

#include <d3d11.h>
#include <d3dx10math.h>
#include <map>

namespace Engine
{
	namespace Graphics
	{
		class AssimpMesh
		{
		public:
			AssimpMesh();
			~AssimpMesh();
			void initialize(char* modelFilename);
			void shutdown();
			void render(unsigned int meshIndex) const;
			const aiScene* getScene() const;
			unsigned int getIndexCountOfMesh(unsigned int i_meshIndex) const;
			ID3D11ShaderResourceView* getTexture(unsigned int i_meshIndex) const;

		private:
			struct VertexType
			{
				D3DXVECTOR3 position;
				D3DXVECTOR2 texture;
				D3DXVECTOR3 normal;
			};

			struct Mesh
			{
				ID3D11Buffer* vertexBuffer;
				ID3D11Buffer* indexBuffer;
				Texture* texture;
				unsigned int indexCount;
			};

			void shutdownBuffers();
			void loadModel(const char* modelFileName);
			void loadTexture();
			void releaseTexture();

			std::map<std::string, Texture*> _textureIdMap;
			Mesh* _meshArray;
			Texture* _defaultTexture;
			const aiScene* _scene;
			Assimp::Importer importer;
		};
	}
}

#endif	//__ASSIMP_MODEL_H__
