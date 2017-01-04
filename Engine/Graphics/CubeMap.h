#ifndef __CUBE_MAP_H__
#define __CUBE_MAP_H__

#include <Engine\Graphics\Texture.h>

#include <d3d11.h>
#include <d3dx10math.h>
#include <stdint.h>

namespace Engine
{
	namespace Graphics
	{
		class CubeMap
		{
			public :

				CubeMap(const char * i_cubeMapPath);
				~CubeMap();
				void uploadVertexBuffers();
				uint8_t getIndexCount();
				ID3D11ShaderResourceView* getTexture();

			private :

				struct VertexType
				{
					D3DXVECTOR3 pos;
				};

				void initializeBuffers();

				ID3D11Buffer *_vertexBuffer, *_indexBuffer;
				Texture* _cubeMap;
				uint8_t _vertexCount, _indexCount;
		};
	}
}

#endif	//__CUBE_MAP_H__
