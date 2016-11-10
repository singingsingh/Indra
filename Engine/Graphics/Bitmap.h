#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <d3d11.h>
#include <d3dx10math.h>

#include <Engine\Graphics\Texture.h>

namespace Engine
{
	namespace Graphics
	{
		class Bitmap
		{
		private:
			struct VertexType
			{
				D3DXVECTOR3 position;
				D3DXVECTOR2 texture;
			};

		public:
			Bitmap();
			~Bitmap();

			bool initialize(int screenWidth, int screenHeight, const char* textureFileName, int bitmapWidth, int bitmapHeight);
			void shutdown();
			bool render(int positionX, int positionY);

			int getIndexCount();
			ID3D11ShaderResourceView* getTexture();

		private:
			bool initializeBuffers();
			void shutdownBuffers();
			bool updateBuffers(int positionX, int positionY);
			void renderBuffers();

			bool loadTexture(const char* textureFileName);
			void releaseTexture();

		private:
			ID3D11Buffer *_vertexBuffer, *_indexBuffer;
			int _vertexCount, _indexCount;
			Texture* _texture;

			int _screenWidth, _screenHeight;
			int _bitmapWidth, _bitmapHeight;
			int _previousPosX, _previousPosY;
		};
	}
}

#endif __BITMAP_H__