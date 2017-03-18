#ifndef __TEXTURE_3D_H__
#define	__TEXTURE_3D_H__

#include <d3dx11tex.h>

namespace Engine
{
	namespace Graphics
	{
		class Texture3D
		{
			public:
				Texture3D(const char* i_fileName, unsigned short i_width, unsigned short i_height, unsigned short i_depth);
				~Texture3D();

				ID3D11ShaderResourceView* getTexture() const;

			private:
				ID3D11ShaderResourceView* _3DTextureSRV;
				ID3D11Texture3D* _3DTextureBuffer;
		};
	}
}

#endif	//__TEXTURE_3D_H__
