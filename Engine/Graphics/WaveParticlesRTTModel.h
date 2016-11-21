#ifndef __WAVE_PARTICLES_RTT_MODEL_H__
#define __WAVE_PARTICLES_RTT_MODEL_H__

#include <Engine\Graphics\Texture.h>

#include <d3d11.h>
#include <d3dx10math.h>

namespace Engine
{
	namespace Graphics
	{
		class WaveParticlesRTTModel
		{
			public:
				WaveParticlesRTTModel();
				~WaveParticlesRTTModel();

				bool initialize(const char * textureFileName);
				void shutdown();
				void render();
				ID3D11ShaderResourceView* getTexture();
				int getVertexCount();

			private :
				struct VertexType
				{
					D3DXVECTOR2 position;
					D3DXVECTOR2 texture;
				};

				bool initializeBuffers();
				void shutdownBuffers();
				bool loadTexture(const char * textureFileName);
				void releaseTexture();

				ID3D11Buffer *_vertexBuffer;
				int _vertexCount;
				Texture* _texture;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__WAVE_PARTICLES_RTT_MODEL_H__