// This class just renders a quad and the wave is built in the pixel shader in the shader BuildWaveShader

#ifndef __BUILD_WAVE_H__
#define __BUILD_WAVE_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <stdint.h>

namespace Engine
{
	namespace Graphics
	{
		class BuildWave
		{
			public:
				BuildWave();
				~BuildWave();
				void render();
				uint8_t getIndexCount();

			private:
				struct VertexType
				{
					D3DXVECTOR2 position;
				};

				ID3D11Buffer *_vertexBuffer, *_indexBuffer;
				uint8_t _indexCount;
		};
	}	// namespace Graphics
}	//	namespace Engine

#endif	//__BUILD_WAVE_H__
