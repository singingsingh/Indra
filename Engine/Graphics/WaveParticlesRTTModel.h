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
				WaveParticlesRTTModel(float i_xMin, float i_xMax, float i_yMin, float i_yMax);
				~WaveParticlesRTTModel();

				bool initialize();
				void shutdown();
				void render();
				int getVertexCount();
				void spawnParticles();
				float getCurrentTime();

			private :

				struct WaveParticle
				{
					D3DXVECTOR2 origin;
					D3DXVECTOR2 direction;
					float spawnTimeMS;
					float actionTimeMS;
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

				enum NextActionType
				{
					SUB_DIVISION,
					REFLECTION
				};

				bool createBuffers();
				void initializeWaveParticlesList();

				void subDivideParticles();
				void reflectParticles();
				void updateBuffers();

				D3DXVECTOR2 reflect(const D3DXVECTOR2& i_incidentVec, const D3DXVECTOR2& i_normalVec);

				WaveParticle* getFreePartices(uint32_t numParticles);
				void pushToList(WaveParticle*  waveParticle, WaveParticle*& i_list);
				void recycleParticles(WaveParticle* waveParticle);
				WaveParticlesRTTModel::NextActionType getNextActionTime(WaveParticle* i_particle);
				char getLineIntersection(const D3DXVECTOR2& p0, const D3DXVECTOR2& p1, const D3DXVECTOR2& p2, const D3DXVECTOR2& p3, D3DXVECTOR2&  i_intersection);

				uint32_t _numParticles, _activeParticles;
				WaveParticle* const _waveParticleMemPool;
				WaveParticle *_freeList, *_subDivList, *_reflecList;

				ID3D11Buffer *_vertexBuffer;
				VertexType* const _vertices;

				float _currentTimeMS;
				float _xMin, _xMax, _yMin, _yMax;
				float _gridWidth;
				float _gridHeight;

				int _numSubDiv, _numReflect;

				D3DXVECTOR2 _topRight, _topLeft, _bottomRight, _bottomLeft;

				float _minAmp;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__WAVE_PARTICLES_RTT_MODEL_H__