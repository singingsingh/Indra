#ifndef __WATER_MODEL_H__
#define __WATER_MODEL_H__

#include <d3d11.h>
#include <d3dx10math.h>

#include <stdint.h>

namespace Engine
{
	namespace Graphics
	{

		class WaterModel
		{
			public:
				WaterModel();
				~WaterModel();
				bool initialize();
				void shutdown();
				void render();
				int getIndexCount();

			private:
				struct VertexType
				{
					D3DXVECTOR3 position;
					D3DXVECTOR3 normal;
				};

				struct WaveParticle
				{
					D3DXVECTOR3 origin;
					D3DXVECTOR3 direction;
					uint64_t spawnTime;
					uint64_t actionTime;
					float amplitude;
					float invRadius;
					float velocity;
					WaveParticle* next;
				};

				bool initializeBuffers();
				void shutdownBuffers();
				void renderBuffers();
				void releaseModel();
				void updateWaveParticles();
				void updateBuffers();
				void initializeWaveParticles();
				WaveParticle* getFreePartices(uint32_t numParticles);
				void recycleParticles(WaveParticle* i_waveParticle);

				ID3D11Buffer *_vertexBuffer, *_indexBuffer;
				int _vertexCount, _indexCount;
				float _gridWidth, _gridHeight;
				uint8_t _gridRows, _gridCols;
				WaveParticle _waveParticle;

				VertexType* _vertices;
				unsigned long* _indices;

				uint32_t _numWaveParticles, _activeParticles;
				const WaveParticle* _waveParticleMemPool;
				WaveParticle *_freeList, *_activeList;
				D3DXVECTOR3 _corner;
		};
	}	// Graphics
}	// Engine

#endif	//__WATER_MODEL_H__