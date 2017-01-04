#ifndef __MODEL_H__
#define __MODEL_H__

#include <d3d11.h>
#include <d3dx10math.h>

namespace Engine
{
	namespace Graphics
	{
		class Model
		{
		private:
			struct VertexType
			{
				D3DXVECTOR3 position;
				D3DXVECTOR4 color;
			};

		public:
			Model();
			Model(const Model&);
			virtual ~Model();
			bool initialize(ID3D11Device*);
			void shutdown();
			void render(ID3D11DeviceContext*);

			int getIndexCount();

		private:
			bool initializeBuffers(ID3D11Device*);
			void shutdownBuffers();
			void renderBuffers(ID3D11DeviceContext*);

		protected:
			ID3D11Buffer *_vertexBuffer, *_indexBuffer;
			int _vertexCount, _indexCount;
		};	// class Model
	}	// namespace Graphics
}	// namespace Engine

#endif	// __MODEL_H__