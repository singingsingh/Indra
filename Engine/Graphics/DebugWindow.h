#ifndef __DEBUG_WINDOW_H__
#define __DEBUG_WINDOW_H__

#include <d3d11.h>
#include <d3dx10math.h>

namespace Engine
{
	namespace Graphics
	{
		class DebugWindow
		{
		private:
			struct VertexType
			{
				D3DXVECTOR3 position;
				D3DXVECTOR2 texture;
			};

		public:
			DebugWindow(int width, int heigth);
			~DebugWindow();

			bool render(int posX, int posY);

			int getIndexCount();

		private:
			void initializeBuffers();
			void updateBuffers(int posX, int posY);
			void renderBuffers();

		private:
			ID3D11Buffer *_vertexBuffer, *_indexBuffer;
			int _vertexCount, _indexCount;
			int _screenWidth, _screenHeight;
			int _bitmapWidth, _bitmapHeight;
			int _previousPosX, _previousPosY;
		};
	}	// namesapce Graphics
}	// namespace Engine

#endif	//__DEBUG_WINDOW_H__