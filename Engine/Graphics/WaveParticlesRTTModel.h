#ifndef __WAVE_PARTICLES_RTT_MODEL_H__
#define __WAVE_PARTICLES_RTT_MODEL_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <stdint.h>

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
				void spawnParticles();

			private :

				struct WaveParticle
				{
					D3DXVECTOR2 origin;
					D3DXVECTOR2 direction;
					uint64_t spawnTick;
					uint64_t actionTick;
					float angle;
					float amplitude;
					float radius;
					float velocity;
					WaveParticle* next;
				};

				struct VertexType
				{
					D3DXVECTOR4 data;
					D3DXVECTOR2 origin;
					D3DXVECTOR2 direction;
				};

				bool createBuffers();
				void initializeWaveParticlesList();

				void subDivideParticles();
				void updateBuffers();

				WaveParticle* getFreePartices(uint32_t numParticles);
				void pushToActiveList(WaveParticle*  waveParticle);
				void recycleParticles(WaveParticle* waveParticle);

				uint32_t _numParticles, _activeParticles;
				WaveParticle* const _waveParticleMemPool;
				WaveParticle *_freeList, *_activeList;

				ID3D11Buffer *_vertexBuffer;
				VertexType* const _vertices;

				uint64_t _currentTick;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__WAVE_PARTICLES_RTT_MODEL_H__