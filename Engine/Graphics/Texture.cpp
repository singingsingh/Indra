#include <Engine\Graphics\Texture.h>
#include <Engine\Graphics\GraphicsDX.h>

namespace Engine
{
	namespace Graphics
	{
		Texture::Texture()
		{
			_texture = 0;
		}

		Texture::~Texture()
		{
		}

		bool Texture::initialize(const char * i_textureFileName)
		{
			HRESULT result;

			// Load the texture in.
			result = D3DX11CreateShaderResourceViewFromFile(GraphicsDX::GetDevice(), i_textureFileName, NULL, NULL, &_texture, NULL);
			if (FAILED(result))
			{
				return false;
			}

			return true;
		}

		void Texture::shutdown()
		{
			// Release the texture resource.
			if (_texture)
			{
				_texture->Release();
				_texture = 0;
			}
		}

		ID3D11ShaderResourceView* Texture::getTexture()
		{
			return _texture;
		}
	}	// namespace Graphics
}	// namespace Engine