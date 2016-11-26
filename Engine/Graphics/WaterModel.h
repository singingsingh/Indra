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
				ID3D11ShaderResourceView* getHeightField();
				void spawnParticles();

			private:
				struct VertexType
				{
					D3DXVECTOR2 pos;
					D3DXVECTOR2 tex;
				};

				bool initializeBuffers();

				void renderBuffers();
				void updateHeightField();
				void buildWaveParticle();

				ID3D11Buffer *_vertexBuffer, *_indexBuffer;
				int _vertexCount, _indexCount;
				uint8_t _gridRows, _gridCols;
				float _gridGap;

				RenderTexture* _singleWaveRTT;
				RenderTexture* _heightFieldRTT;

				D3DXVECTOR2 _corner;

				WaveParticlesRTTModel* _waveParticlesRTTModel;
				WaveParticlesRTTShader* _waveParticlesRTTShader;

				ID3D11BlendState *_additiveBlending, *_prevBlendingState;
				ID3D11DepthStencilState* _disableDepthStencil, *_prevDepthStencil;
		};
	}	// Graphics
}	// Engine

#endif	//__WATER_MODEL_H__