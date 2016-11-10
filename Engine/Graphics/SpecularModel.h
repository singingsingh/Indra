#ifndef __SPECULAR_MODEL_H__
#define __SPECULAR_MODEL_H__

#include <Engine\Graphics\Texture.h>

#include <d3d11.h>
#include <d3dx10math.h>

namespace Engine
{
	namespace Graphics
	{

		class SpecularModel
		{
			public:
				SpecularModel();
				~SpecularModel();
				bool initialize(char* modelFilename, const char* textureFilename);
				void shutdown();
				void render();
				int getIndexCount();
				ID3D11ShaderResourceView* getTexture();

			private:
				struct VertexType
				{
					D3DXVECTOR3 position;
					D3DXVECTOR2 texture;
					D3DXVECTOR3 normal;
				};

				struct AssimpData
				{
					float x, y, z;
					float tu, tv;
					float nx, ny, nz;
				};

				bool initializeBuffers();
				void shutdownBuffers();
				void renderBuffers();
				bool loadModel(const char* modelFileName);
				void releaseModel();
				bool loadTexture(const char* textureFileName);
				void releaseTexture();

				ID3D11Buffer *_vertexBuffer, *_indexBuffer;
				int _vertexCount, _indexCount;
				Texture* _texture;
				unsigned long* _index;
				AssimpData* _assimpData;
		};
	}	// Graphics
}	// Engine

#endif	//__SPECULAR_MODEL_H__