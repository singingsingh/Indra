#ifndef __WATER_MODEL_H__
#define __WATER_MODEL_H__

#include <Engine\Graphics\RenderTexture.h>
#include <Engine\Graphics\WaveParticlesRTTModel.h>
#include <Engine\Graphics\WaveParticlesRTTShader.h>

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
				void spawnParticles();
				ID3D11ShaderResourceView* getHeightField();

			private:
				struct VertexType
				{
					D3DXVECTOR2 pos;
					D3DXVECTOR2 tex;
				};

				struct WaveParticle
				{
					D3DXVECTOR3 origin;
					D3DXVECTOR3 direction;
					uint64_t spawnTick;
					uint64_t actionTick;
					float angle;
					float amplitude;
					float radius;
					float velocity;
					WaveParticle* next;
				};

				bool initializeBuffers();
				void shutdownBuffers();
				void renderBuffers();
				void subDivideParticles();
				void updateBuffers();
				void updateHeightField();
				void initializeWaveParticlesList();
				WaveParticle* getFreePartices(uint32_t numParticles);
				void pushToActiveList( WaveParticle*  waveParticle);
				void recycleParticles(WaveParticle* waveParticle);
				void buildWaveParticle();

				ID3D11Buffer *_vertexBuffer, *_indexBuffer;
				int _vertexCount, _indexCount;
				uint8_t _gridRows, _gridCols;
				
				const WaveParticle* _waveParticleMemPool;
				RenderTexture* _singleWave;
				float _gridGap;
				RenderTexture* _heightFieldRTT;

				VertexType* _vertices;
				unsigned long* _indices;

				uint32_t _numWaveParticles, _activeParticles;
				WaveParticle *_freeList, *_activeList;
				D3DXVECTOR2 _corner;

				WaveParticlesRTTModel* _waveParticlesRTTModel;
				WaveParticlesRTTShader* _waveParticlesRTTShader;

				ID3D11BlendState *_additiveBlending, *_prevBlendingState;
				ID3D11DepthStencilState* _disableDepthStencil, *_prevDepthStencil;
		};
	}	// Graphics
}	// Engine

#endif	//__WATER_MODEL_H__