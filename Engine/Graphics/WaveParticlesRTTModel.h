#ifndef __WAVE_PARTICLES_RTT_MODEL_H__
#define __WAVE_PARTICLES_RTT_MODEL_H__

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

				bool initialize();
				void shutdown();
				void render();
				int getVertexCount();

			private :
				struct VertexType
				{
					D3DXVECTOR2 position;
					D3DXVECTOR2 size;
				};

				bool initializeBuffers();
				void shutdownBuffers();

				ID3D11Buffer *_vertexBuffer;
				int _vertexCount;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__WAVE_PARTICLES_RTT_MODEL_H__