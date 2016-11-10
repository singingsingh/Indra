#ifndef __DIFFUSE_MODEL_H__
#define __DIFFUSE_MODEL_H__

#include <Engine\Graphics\Texture.h>

#include <d3d11.h>
#include <d3dx10math.h>

namespace Engine
{
	namespace Graphics
	{
		class DiffuseModel
		{
			public:
				DiffuseModel();
				~DiffuseModel();

				bool initialize(const char * textureFileName);
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

				bool initializeBuffers();
				void shutdownBuffers();
				void renderBuffers();

				bool loadTexture(const char * textureFileName);
				void releaseTexture();

				ID3D11Buffer *_vertexBuffer, *_indexBuffer;
				int _vertexCount, _indexCount;
				Texture* _texture;
		};
	}
}

#endif	//__DIFFUSE_MODEL_H__