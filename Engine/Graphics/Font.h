#ifndef __FONT_H__
#define __FONT_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>

#include <Engine\Graphics\Texture.h>

namespace Engine
{
	namespace Graphics
	{
		class Font
		{
		public:
			Font();
			~Font();

			bool initialize(const char* fontDataFileName, const char * textureFileName);
			void shutdown();

			ID3D11ShaderResourceView* getTexture();
			void buildVertexArray(void* vertexArrayBuffer, const char* sentence, float drawX, float drawY);

		private:
			struct FontType
			{
				float left, right;
				int size;
			};

			struct VertexType
			{
				D3DXVECTOR3 position;
				D3DXVECTOR2 texture;
			};

			bool loadFontData(const char* fontDataFile);
			void releaseFontData();
			bool loadTexture(const char* fontTextureFileName);
			void releaseTexture();

			FontType* _font;
			Texture* _texture;
		};
	}	// Graphics
}	// Engine

#endif	//__FONT_H__