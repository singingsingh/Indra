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

				bool initializeBuffers();
				void shutdownBuffers();
				void renderBuffers();
				void releaseModel();

				ID3D11Buffer *_vertexBuffer, *_indexBuffer;
				int _vertexCount, _indexCount;
				float _gridWidth, _gridHeight;
				uint8_t _gridRows, _gridCols;
		};
	}	// Graphics
}	// Engine

#endif	//__WATER_MODEL_H__