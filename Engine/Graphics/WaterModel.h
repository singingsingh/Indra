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
					D3DXVECTOR2 texture;
					D3DXVECTOR3 normal;
				};

				struct WaterData
				{
					float x, y, z;
					float tu, tv;
					float nx, ny, nz;
				};

				bool initializeBuffers();
				void shutdownBuffers();
				void renderBuffers();
				void releaseModel();

				ID3D11Buffer *_vertexBuffer, *_indexBuffer;
				int _vertexCount, _indexCount;
				unsigned long* _index;
				WaterData* _waterData;
				uint8_t _gridWidth, _gridHeight;
				uint8_t _gridRows, _gridCols;
		};
	}	// Graphics
}	// Engine

#endif	//__WATER_MODEL_H__