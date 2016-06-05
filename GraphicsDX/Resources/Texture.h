#ifndef __TEXTURE_H__
#define	__TEXTURE_H__

#include <d3d11.h>

namespace GraphicsDX
{
	class Texture
	{
		public:

			static Texture * CreateTexture(void * i_pTextureData, size_t i_sizeTextureData, const char * texFileName);
			static void Release(Texture* &i_texture);

			ID3D11ShaderResourceView* getShaderResourse();
			bool getDimensions(unsigned int & o_Width, unsigned int & o_Height, unsigned int & o_Depth);

		private :

			ID3D11Resource*			m_pTextureResource;
			ID3D11ShaderResourceView*	m_pShaderResource;
			const char * texFileName;

			Texture(ID3D11Resource* i_pTextureResource, ID3D11ShaderResourceView* i_pShaderResource, const char * texFileName);

	};	// class Texture
} // namespace GraphicsDX

#endif //__TEXTURE_H__