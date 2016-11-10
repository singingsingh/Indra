////////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <d3d11.h>
#include <d3dx11tex.h>


namespace Engine
{
	namespace Graphics
	{
		class Texture
		{
		public:
			Texture();
			Texture(const Texture&);
			~Texture();

			bool initialize(const char *);
			void shutdown();

			ID3D11ShaderResourceView* getTexture();

		private:
			ID3D11ShaderResourceView* _texture;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__TEXTURE_H__